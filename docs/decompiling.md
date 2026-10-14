# Decompiling notes

Lessons from getting functions to match exactly. Compiler and flags: see `compiler.md`.

## Workflow

1. Pick a function or file. `ninja` then `tools/bin/dsd dis -c config/usa/arm9/config.yaml -a build/usa/asm --ual`
   gives disassembly to read from.
2. For a single function, iterate in a scratch file with a `// PROBE: <mangled symbol> <module> <address>` line:
   `python3 tools/probe_compilers.py <file> --only 2.0/sp2p2 --flags='-O4,p -ipa file -inline on,noauto' --show`
   prints a side-by-side diff with relocated words masked. When only register names or stack slots differ, mark
   the declarations that may move with a trailing `//PERM` and run `python3 tools/permute.py <file> <function>`:
   mwcc allocates in declaration order, and the tool tries the orders (all of them for up to 8 lines).
3. Put the code in its translation unit under `src/`, with an entry in `config/usa/arm9/**/delinks.txt` (see
   `CONTRIBUTING.md`). Shared classes go in `include/`, SDK declarations in `libs/nds/include/`.
4. Compile it (`ninja build/usa/src/<path>.o`), then rename the original symbols to the names your code uses:
   `python3 tools/sync_symbols.py build/usa/src/<path>.o <module> --write`. The tool locates each matching function
   by its bytes and reads every referenced address from the original code, so `symbols.txt` follows the source.
   A non-matching function of the same size can be placed with `--assume <mangled>=<addr>`, and
   `--assume-embedded` places every `func_<addr>`/`func_ovNNN_<addr>` at the address in its name. Only exact matches
   have their referenced symbols renamed: a near-miss's literal pool can be laid out differently.
5. Statics of inline functions (`_ZGVZ…`, `_ZZ…`, e.g. singleton instances and guards) are defined by every file
   that uses them. Mark them `weak` in `symbols.txt`. Compiled objects get their copies demoted to weak as well
   (`tools/demote_multidef.py`, run by the build), so main's original copy wins when a file is linked.
6. Run `python3 tools/check_calls.py`. Matching masks relocated words, so a function can match while calling the
   wrong function (e.g. `_fmul` where the original calls `_fdiv`); this compares every call in fully matching
   functions with the original's branch targets.
7. `ninja objdiff report` shows per-function and per-section results. When everything matches, link the unit:
   - `python3 tools/link_check.py <unit>` checks the function order (mwcc emits functions in source order; fix it
     with `tools/reorder_functions.py <unit> --write` then `--fix-decls <unit>`) and that every compiled data
     section has a range in the delinks entry.
   - `python3 tools/data_ranges.py <unit> [--write]` finds those ranges: relocations in exactly matching
     functions give the original address of each data section the code uses, and the data sections found point
     at the rest. It refuses when sections disagree, land outside the module's section of that kind (e.g.
     `.data` in the original's `.rodata`: the source has the const-ness wrong), are laid out in a different
     order, or hold different bytes. It also adds the boundary symbols dsd needs.
   - `python3 tools/mark_complete.py <unit>`, `ninja`: the ROM must still match.
   - `python3 tools/sync_linked_symbols.py --write`, then `ninja check`: inside complete units, symbols.txt takes
     the linked binary's names (`@N` literals, statics marked `local`) and binding.

A function that doesn't match yet is kept in source with a `// non-matching` comment describing the difference.

## Known open problems

- The GameOverScene/BattleUnionAbility/BattleUnk0227532c "literal order" problem is resolved: GameOverScene's
  tables come before most of its includes; the other two were loading the wrong file names (probes mask string
  addresses, so a wrong literal can look like a match).

- **Fully matched units that don't link yet** (2026-10-14):

- **Game.cpp doesn't link yet (`main`, then its `.bss`).** Every function matches. Found while trying (2026-10-14):
  - `main` (0x02000c04) is Game's but sits right after the start-up code.
    Placing it works: a stub unit for 0x02000c04-0x02000cb4 whose LCF line is rewritten to
    `OBJECT(main, Game.o)` links and puts `main` there.
  - The singleton destructors must be `inline` in Game.cpp (done): they then come out as weak copies after
    `func_02020328` in its first-use order, which is the original function order at 0x02020a08-0x02020a44.
  - Game's copies of the singleton guards (0x020c17a8-0x020c17e0) and instances (0x020c1870-0x020d42e8) are the
    ones the original kept, so Game's `.bss` range must cover them. Then two names other files use for fields
    inside instances need linker-script definitions: `data_020c1a4c` (TextManager instance + 0x38) and
    `data_020c3938` (WirelessManager instance + 0x908). Both were tested and link.
  - SceneRunner is 0x14 bytes (done), and `UnkStruct_020c17a8` is at most 4 bytes: its instance at 0x020c17a8 is
    followed by a guard at 0x020c17ac. TitleScene's solved layout only works with it at 16 bytes or more, so
    TitleScene's layout has to be solved again first (its model doesn't reproduce; a brute force over singleton
    include order and table position found nothing).
  - The 13 guards still come out in another tie order; `layout_search.py --weak` (places a unit's own guards and
    instances) finds no order with the current participants.

- **Two literal-pool entries for one address.** `func_02023504` (FileSystem.cpp) loads 0x020d53e4 from two
  different pool words, so two relocations (different symbols, or symbol plus offset) resolve to the same address
  in the original. The source models it with a second extern, `data_020d53e4_h`; the real layout of the loader's
  statics (header word, registration node, two MI contexts, state fields) is still to be worked out.
- **Order of identical string literals (solved).** Copies of one literal in a table's initializer are ordinary
  participants of the file's size sort (below), so their order follows from the whole list. `tools/layout_search.py`
  models that sort and finds the include and definition orders that reproduce the original (ArenaScene's five window
  paths, UnkStruct_02070608's four).
- **Fitting the size sort by hand (BattleCommandMap).** When `layout_search.py` refuses a file (its model misses a
  participant, such as a local array's initializer `@N` in `.rodata`, which joins at its function's position), a
  small script does the job: list the participants with sizes (strings of a table's initializer just before it, the
  vtable last, the singletons' guard, node and instance at the top), check that `data_order.heapsort` on the
  reversed list reproduces the current object for some order of the header objects, then hill-climb the static
  definition order against the original, keeping each static before its first use. The first order found compiled
  to the original layout of all three sections. Defining every table in the range matters: with only the `.data`
  tables defined, no order gave the original string order, because the `.rodata` tables take part in the same sort.
- **Merging tables changes what the size sort can reach (MyroomGiftScene).** With the position table (a7c) and the
  three 12-byte order tables as separate objects no definition order gave the original `.rodata`; as one 60-byte
  struct (`GiftPos pos[3][4]; u8 order[12]; u8 ids[12]; s8 slots[12];`) it does, and the folded `+1` of `.pos[r][c].y`
  reproduces the original's a7d pool word. A two-byte `{x, y}` struct instead of `u8[2]` keeps a54 out of its own
  section. Pool order in func_ov008_0221de34 came from building the pointer inside the loop
  (`const u8 *ids = t.ids; ids += i;`).
- **Function order is definition order.** g3d_res matched but its ROM differed once linked: `func_02033788` was
  defined before its caller `func_02033714` (for the declaration), so mwcc emitted it first. A prototype and the
  definition after the caller fixed it.
- **Literals before a vtable mean an earlier file (MessageWindow).** Function literals are emitted after a file's
  shared `.data` section, so UnkStruct_020651bc's six path strings lying before MessageWindow's vtable showed the
  two classes were separate files; split at 0x0206550c, both link.
- **Many statics: `layout_search.py --climb N`.** Permuting more than about 7 statics with 5 singletons is too slow;
  `--climb 300` hill-climbs singleton order, static order and the split instead. AttrSelScene (8 tables, 5
  singletons) linked with its first answer: TextManager, TouchManager, SpriteManager and InputManager headers, then the
  tables, then SoundManager's.
- **Statics between includes (UnkStruct_02076058).** Defining its three window tables reordered the four
  singleton nodes in `.bss`. `layout_search.py` lists working orders with a cost (how many things move, not how many
  objects are off); the one used defines the tables right after the first header (TouchManager), with
  `UI/SelectWindow.hpp` included early for the record types since it pulls in no singletons.
- **BattleCharaAct's data layout (open).** All functions match and the data is defined from the original bytes,
  but `layout_search.py`-style modelling finds no declaration order: in the original the 17 16-byte rows follow the
  17 32-byte arrays (only possible if they and the 0x28-byte rows at 0x0227af68 are one 592-byte object), and the
  17 name strings come out in an order their table's initializer can't produce (named arrays would allow it, but
  only declared after the table or interleaved with the two VecFx32s).
- **Guard load before a preceding store (solved).** mwcc lets a following load (e.g. the next singleton's guard)
  move above a store only when the store's offset is below half of the declared size of the object stored into.
  `TextManager` was declared 0x5c bytes, so the `mUnk_5b` store (offset 0x5b) blocked it; with its real size,
  0xe8 (an unused trailing member, so `Widths()` keeps its code), 22 functions match. So when a store seems to
  pin a later load, check the declared size of the stored-to object.

- **Both CameraManager mode loads before the compares.** `(Get().mMode == 2) | (Get().mMode == 3)`
  (BattleCardEffect::func_ov007_02271888, UnkStruct_02078290::Update and func_020785d8): the original loads the
  instance address, then both modes, then compares (first mode and its 0/1 result share r1); we compare the first
  mode before loading the second. Every form tried compiles to the same code: operand order, `||`, negations,
  temporaries before or after the second `Get()`, references, pointers, inline wrappers around `Get()` or the
  compares, and `mMode` as volatile, unsigned or at another offset. No other 2.0 build differs.
- **objdiff and calls inside a unit.** A `bl` to another function of the same unit counts as a mismatch while that
  callee does not match, even though name and addend agree; such a caller reaches 100% once the callee does.
- **Call checks.** `tools/check_calls.py` compares every call in matching functions with the original;
  `tools/check_callees.py` compares, per function (matching or not), the set of callees with the `bl` targets in
  the original's range. Probes and objdiff mask call targets, so both are needed: a near-identical neighbour
  (`DC_StoreRange`/`DC_FlushRange`, `MI_CpuCopy8`/`MIi_CpuCopyFast`, signed/unsigned division) is easy to call by
  mistake. It skips `this`-adjusting thunks and ignores branch-like literal-pool words; a clean run reports 0.
  `tools/check_strings.py` does the same for string literals in matching functions (a wrong file name or a case
  difference in a path still "matches"). Run all three before committing.

## Data layout within a section

mwcc does not lay out a file's objects in declaration order. Found by experiment (and checked against 14 random
layouts), for the objects of one section kind (`.bss`, and likely `.data`/`.rodata` the same way):

1. Objects join a list as they are declared, by prepending, so the list ends up in reverse declaration order.
2. A *used* singleton accessor (`static T &Get() { static T instance; ... }`) adds its guard (4 bytes), its
   destructor-chain node `@N` (12 bytes, only when `T` has a destructor) and its instance where the inline
   function is parsed, i.e. where its header is first included. An unused one adds nothing. File-level statics
   add themselves where they are defined.
3. The list is heapsorted by size (max-heap, strict comparisons): ascending sizes, with equal sizes in an
   order that depends on the whole list.
4. Weak objects (guards, instances) then move to their own sections; what stays is the unit's layout.

So header include order and the definition order of statics decide the layout, and an object of a smaller
size can never follow a bigger one from the same list. The synthetic rule above is exact, but real files have more
participants than it accounts for (function-local statics with destructors, objects from other sections), so
predictions from `tools/data_order.py` are only a hint. `python3 tools/layout_search.py <unit>` is the model that
works: one list for the whole file (`.bss`, `.data` and `.rodata` together; each shared section is emitted at the place
of its first member), each singleton contributing node, guard, instance or guard, node, instance (calibrated per
singleton against the compiled object), each static preceded by its initializer's literals, vtables last. It refuses
to search unless it reproduces the current compile, then searches singleton order, static order and where the statics
sit among the headers, offline. `python3 tools/fix_layout.py <unit>` tries header orders by compiling them.
`.clang-format` keeps include
order (`SortIncludes: Never`), and it is load-bearing.

Other findings from linking:
- **Two classes, one file.** The ConfrontEmblem0N classes were compiled in the same file as their
  ConfrontBg0N/Confront03 partner: the emblem's strings sit in the partner's data block, after both vtables.
  An object that lies in a neighbour's block in the original means the code using it belongs to that file.
- **Dead constructor copies.** mwcc emits separate C1 and C2 constructors (and D1/D2) for some classes. The
  original link dropped the unused copy and the strings only it used; `data_ranges.py` ignores sections used only
  by functions the original doesn't have.
- **Stripped weak copies in other files.** An inline destructor (e.g. `virtual ~WirelessManager() {}`) is emitted
  as a weak copy in every file that uses the singleton's `Get()`; the original link kept one (Game.cpp's, at
  0x02020a2c) and dropped the rest. Our objects still carry the copy, which shifts objdiff's view of later
  relocations: ArenaScene::func_ov001_022197ec, ArenaWindowScene::Update, BattleCardSpec::Update and
  BattleCharaAct::Update show 99.9x% with identical code. The ROM is unaffected.
- When no include order works, look for a boundary problem before anything else (IBattle: its first functions use
  a WirelessManager node that lies among the previous file's nodes).

- **Rules from the layout study (build trials on synthetic files):**
  - A singleton's three objects join in the order guard, node, instance if the class has a non-trivial
    constructor (user constructor, virtuals, or a base/member with one), and node, guard, instance if trivial.
  - Vtables join at the end of the file, in reverse class order, and only those the file emits (a key function
    defined mid-file still puts its vtable at the end).
  - String literals in a table's initializer join just before the table; literals inside functions don't join.
    Function-local static tables join at their function's position.
  - Separate sections: objects the file never references (inline accessors in included headers count as
    references), byte-aligned const tables whose size is a multiple of 4, function-local mutable statics, and every
    string literal. Other referenced objects share one section per kind; mwld starts each section on 4 bytes.
  - Unit boundaries show in the nodes: the same singleton's node at two original addresses means two files; a
    smaller object between two larger ones means a boundary; another unit's data on both sides means a shared file.
  - A function that is only `operator delete(p); return p;` is a deleting destructor and belongs to its class's file
    (SceneRunner's sat at the start of FrameUpdate's range).

- **Unreferenced data and the link.** Unreferenced global objects take part in the size sort and get their own
  sections; unreferenced statics are dropped and don't. The original link kept unreferenced data, but our mwld
  `-dead` strips it once a unit is linked from source, so such objects go in `config/usa/arm9/force_active.txt`.
  When data_ranges can't place a unit (nothing references some sections), write the ranges by hand and check the
  layout byte by byte.

- **Data no relocation points at can still be referenced.** A const table read only with constant indices is
  folded into immediates, so no code relocation reaches it, yet it stays referenced and joins the shared section.
  StoreScene's 0x24 bytes between its 7-byte table and `sMenuItems` are the three event-camera position rows that
  `Update` passes to `func_020702e0` as immediates; declaring `sEventPositions[3][6]` after `sMenuCells` and reading
  it there fixed `.rodata` and `.data` at once. An unused inline accessor does not count as a reference. Look for a
  gap's values among a function's immediates before writing it off as dead data.
- **Char arrays always get their own section**, even when the file references them; a struct of the same size
  doesn't (SpriteFade: `static s8 sFade[2]`, index 0 main screen, 1 sub).
- A weak inline vtable copy needs its unit's `.data` range set by hand and the symbol marked `weak` (SceneBase).

- **Seeing the order before scheduling.** Compiling the same file with `-proc generic` instead of `-proc arm946e`
  (other flags unchanged) skips the ARM9 instruction scheduler, which runs before register allocation. Comparing
  the two shows whether a register difference comes from scheduling (an instruction moved past a definition) or
  from allocation itself. BattleResult::func_ov007_0224f9a0's last 5 words are a scheduling difference: the
  original finishes the third Draw's y before loading `this`, which no source form tried reproduces.

## Source patterns that matter

- **`x[n] = v; n++;` vs `x[n++] = v;`** schedule differently; BattleCpu::func_ov007_02231300 needs the split form. A
  counter also passed to `MATH_Rand32` matched as `u32 n` with `n = (u8) (n + 1)` (func_ov007_022329cc).
- **Re-read instead of caching.** Where the original reloads a table element or a global chain in a loop, read it at
  each use rather than through a local pointer (func_ov007_02231300, func_ov007_022314bc).

- **`p = e; p += idx;` splits the address CSE.** Built that way, only `idx * size` is shared and the base is re-added
  at each use (`ldrh r0, [r6, r7]`); `&e[idx]` or `e + idx` shares the whole address (BattleConfrontCard::func_ov007_022428d4).
- **Declared before, computed after.** Declaring `opp`/`oppLevel` before `self`/`selfLevel` but computing the self
  chain first changes which value is spilled, independently of either order alone (same function).

- **Zeros from one declaration share a register.** `s32 done = 0, advance = 0, pending = 0;` as one declaration lets
  the three zeros share one register, where separate declarations (or `= done`) don't (UnkStruct_02065c40::func_02066814).
- **`u8 now = ++field;`** gives `add; and; strb` with no reload of the field; `field++; now = field;` reloads it.
- **A chained assignment as an argument:** `SetAnimation(mUnk_14f2 = value)` schedules the store differently from two
  statements (func_0206a1c8).

- **Parameter width shows in the caller.** Passing a `u8` to a `u32` parameter makes a conversion temporary that
  shifts the caller's registers; FieldSelectScene's `func_ov001_022267d0(u8)` fixed two callers' loops.
- **A pointer kept only part of the way is a second `Get()`.** When the original holds a SpriteScreen (or other
  singleton part) in a register through some loops and then reloads it from the pool, the later uses come from a fresh
  `SpriteManager::Get().Screen(n)` (FieldSelectScene::func_ov001_0221fa94: 1082 differing words to 0).
- **A local array filled from a one-use rodata table is an aggregate initializer.** `u8 buf[3] = {0, 1, 4};` makes
  mwcc emit an anonymous rodata object (often first in the unit's .rodata) and changes which temporary is created
  first; copying `buf[i] = data_xxx[i]` element by element does not match (TitleScene::Update, BattleTarget::Update).
- **Check a callee's real parameter type before fighting a caller's registers.** An `s16` local passed to a `u32`
  parameter inside a loop is widened by a hoisted temporary that mwcc coalesces with the local, so declaration order
  stops affecting it. With `func_02027778`'s third parameter as `s16` (its prologue then matches too), MyroomGiftScene
  and MyroomShootScene matched by declaration order alone.
- **Repeated calls with constant arguments schedule in a cycle.** In a lab of 20 identical `func_02025b64` calls the
  argument-setup pattern repeats every 7 calls; an enum-typed zero (`LoadArg z`, in System/FileSystem.hpp) for one
  call's last three arguments shifts it (MyroomGiftScene::func_ov008_0221de34, UnkStruct_02071d24::func_02072734).
- **A stack parameter the callee reads as `*(s16 *)&param` is really `s16`.** Declaring it so makes callers narrow at
  the call, which creates the value later and gives it the low register: with
  `SpriteScreen::func_02027778(..., s16 unk4, ...)`, `int y = row * 0xc + base;` passed straight through matched
  TournamentScene::func_ov002_0221cd10. The callee's other narrow reads (`*(u16 *)&unk6` and so on) are untested.
- **A pointer local can hoist shared index arithmetic over a branch.** `BattlePlayer *pl = list->mPlayers[p];`
  makes `p * 4` a common subexpression with a later `p * 4 + 0x28`, and mwcc hoists it above the `if`. Keeping the
  container in a local and indexing inside each arm (`list->mPlayers[p]->mUnk_f1`) stops it
  (BattleCommand::func_ov007_0223a734). A `(u8 *)` cast store recolours differently from a real `u8` field store.
- **An enum-typed result stops the return being folded.** `UnitCompare result` kept a `return result` that a `u32`
  version folded into `mov r0, #0` (BattleUnk022637ac's func_ov007_02267884). Reading components into named locals
  in the original's order (`z = p->z; y = p->y; x = p->x;`) fixes the load order before a call.
- **`delete p;` versus `if (p) delete p;`.** mwcc emits the null check for a plain `delete` itself; an explicit `if`
  changes the block shape and the scheduling around it (BattleResult's destructors). Try both: BattleConfrontCard's
  func_0223f398 needed the explicit `if`.
- **`stm` where the original has two `str`s for stack arguments.** Declare the flag without an initialiser and assign
  it only in the arm that needs it (`BOOL has; if (...) {...} else { has = 0; ...}`), passing literals in the other
  arm (BattleCommandList::func_0205a804).
- **A byte array that is really an array of small structs.** `u8 mUnk_0c[10]` used as `{cur, target}` pairs: struct
  indexing (`mUnk_0c[i].mCur`) stops mwcc sharing the address across a call, which frees a register for hoisting.
- **From the FieldSelectScene pass.**
  - `u32 k = 0; ... k = (u8) (k + 1);` fixed a counter/coordinate register swap where `u8 k` in every order failed
    (the TournamentScene idiom again).
  - `const u8 *p = tbl; p += k * 3; p[byte]` stops mwcc reassociating `tbl[k * 3 + byte]` into a hoisted
    `tbl + byte`; every single-expression form hoists.
  - `byte = f / 8` gives `lsr` where `f >> 3` gives `asr` (on a signed `f`).
  - A value the original keeps across a loop without reloading, written in place, is a temporary; a named local
    shifts registers.
  - `switch` instead of an if-chain stops mwcc predicating the arms.
- **An `&` chain no single expression reproduces.** `BOOL lo = (a >= 0) & (b >= 0); if (lo & (a < 0x100) & ...)`
  gave the original's evaluation order where none of the 120 one-expression forms did (BattleShoot::func_ov007_0226f76c).
  Reading `tbl[idx]` into a local before an `f++` puts the table load ahead of the store, and `if (field)` right after
  `field = var;` compiles to `tst #255`.
- **Arguments with side effects are evaluated before the stack-argument stores.** A volatile read or `x++` in an
  argument is computed before the `[sp]` stores and the other arguments; every side-effect-free form is folded into
  the call and loaded after the stores, because the scheduler never lifts a `[this]` load above a `[sp]` store.
  `Screen(*(volatile u8 *) &mScreen)` is the placeholder used in MessageWindow::func_020658d0. A value whose only
  consumer is one later call is substituted into that call; a compound assignment or a later read keeps it in place.
- **`(u8)` casts at a call site often stand in for the callee's parameter types.** Making `func_020231f4` take
  `u8, u8` matched three callers in UnkStruct_02078e14_b.cpp. An `extern` object's declared size can matter too:
  stores at 0x1c/0x1d into `UnkStruct_0220f540` scheduled like the original only once the type was at least 0x3c bytes.
- **Code that leaves no instructions can still move the scheduler.** The choice between equally ready instructions
  depends on where they sit in the block; dummy calls before a block flip such ties (func_020843ac, func_02085220).
- **A negative constant stored with `strh` means a signed 16-bit destination.** `mov r0, #0xe000; sub r1, r0, #0x12000`
  (0xffffc000) stored to a halfword can't come from a `u16` field: every form folds to 0xc000. Only a signed 16-bit
  lvalue gives it (`*(s16 *) &p->mRotY = -0x4000;` as a placeholder, BattleCardCamera::func_ov007_02244bb4).
- **A wrong constant can look like a scheduling difference.** When the original builds a value from a neighbouring
  constant (`mov r7, #34; sub r7, r7, #47`), that shows the true value (-13 where the source had 3,
  UnkStruct_02065c40::func_0206fa20). Likewise a literal-pool word that differs is often a wrong divisor or constant
  (BattleCommand's `/ 24`).
- **A table pointer created before the index load** (`e = tbl; id = ...; ... e[id]; e += id;`) colours differently
  from `e = &tbl[id]`.
- **From the BattleConfrontCard pass.**
  - Gaps in a jump table that lead to a label inside the switch are case labels the source is missing (not the
    default).
  - `x *= 10; x &= 0xffff;` keeps a call result and its narrowed product in one callee-saved register
    (`movs rN, r0`); `x = (u16) (x * 10)` splits them.
  - A pointer built by `p = base; p += idx;` is not substituted, so identical `base[idx]` accesses elsewhere don't
    share its address.
  - Reading a member directly (`mData->x`) rather than through a `T *data = mData;` local makes the pointer a
    temporary; it is still loaded once when no store or call comes between the reads.
- **From the BattleTarget pass.**
  - A narrow (`u8`) stack parameter is loaded first in the prologue; `BOOL` puts it later (two functions fixed by
    `u8 flag`).
  - A `u16` local spilled to the stack is reloaded before each store of the zero it shares; `s32`/`u32`/`int` share
    one register for all the stores.
  - `VecFx32 v = {0, 0, 0};` stores through an `add rN, sp, #off` base; field stores and `SetVec` use sp offsets.
  - `if (n) { i = 0; do ... while (++i < n); }` has one guard; a `for` loop keeps a second `cmp`/`bls`.
  - Re-reading a global chain (`g.list->mPlayers[k]->a + g.list->mPlayers[k]->b`) forces fresh loads where a named
    pointer lets mwcc reuse earlier ones.
  - A scalar argument of an inline setter is loaded before the struct copy in its body
    (`SetSphere(s, &pos, radius)`).
  - `(n + 1) * x; ...; n++;` keeps the unmasked `n + 1` in the multiply.
  - Where the original loads every component before storing, write the store as `SetVec(...)` or a struct copy.
- **Which tables are addressed through their own symbol (verified in a lab).** mwcc's object writer puts every
  *referenced* initialised (or .bss) object into one shared `.data` section; unreferenced objects each get their own.
  A load or store at a constant offset into an object in the shared section is addressed from the section base
  (re-anchored at `off & ~0xff` for strh, `& ~0xfff` for str), so two tables can share one pool word; taking the
  address as a value or indexing with a variable keeps the symbol. Objects that stay out of the shared section, and
  so are addressed through their own symbol, are: `extern` objects; arrays of char/u8/s8 (own sections, like
  strings); weak objects (function-local statics of `inline` functions, template statics); and objects under
  `#pragma section npdata begin`/`end`. Nothing else matters (size, alignment, definition order, static or global,
  optimisation level). `const` always wins: a const object goes to `.rodata`. So when the original loads a table
  defined in the file through its own pool word, define it as a function-local static of an inline accessor
  (`inline WindowItemRecord *ListItems() { static WindowItemRecord sItems[4] = {...}; return sItems; }`, as in
  BattleCommand and BattleCommandList).
- **A value set in both arms of an if/else is coloured after `this`; the same if/else in an inline member that returns
  it gets the lowest register** (ParkMenu::DrawText's `u32 entry = TextEntry(i);`). The helper must keep the
  `u32 entry; if (...) {...} else {...} return entry;` shape; an early return changes it.
- **A macro where an inline won't do.** `#define TEST_BIT(f, i) ((f)[(i) / 8] & (1 << ((i) % 8)))` (and a matching
  `SET_BIT`) lets mwcc reuse the loaded byte for a following set, which the inline helpers don't
  (MyroomTipsScene::func_ov008_022274f8). A caller passing an `ldrsb` value with no `and` means the parameter is wider
  than `u8` (`func_0206f564(s32 unk1, ...)`).
- **Reused locals are coloured in order of first appearance.** When a function-scope local is reused in several
  loops, mwcc colours its later live ranges in the order the variables first *appear* in the function, not their
  declaration order; a dead `j = 0;` near the top (or moving `g = 0;` above other assignments) reorders them
  (func_02089278 in ParkCharacters.cpp). Separate variables still go by declaration order, ahead of those ranges. A
  `u8` passed to `MATH_Rand32` merges into the inline's 64-bit temporary, so its declaration position doesn't matter.
- **One loop counter for every loop.** Original functions often reuse one function-scope `u8 i` (and a second `j`)
  for all their loops, even where one counts down; per-loop locals colour differently, and no declaration order
  fixes that. A counter shared by several loops is coloured above a hoisted singleton address, separate per-loop
  counters below it (BattleHud::func_ov007_02227c54, BattleBase::func_ov007_0221d088). When two counters step together,
  the order of `j++; i++;` matters too.
- **Chained member assignments in constructors.** `a = b = 0;` reproduces the reload-and-truncate sequences
  (`ldrb`/`and #0xff`) that separate stores don't (BattleBase's constructor).
- **An inline accessor turns a field read into a temporary.** `mUnk_2c->Entries()` instead of `mUnk_2c->mEntries`
  moved a pointer from r5 to r6 (BattleBase::Update), like `CurrentPlayer()` and `Screen(n)`.
- **Spill slots.** Spilled parameters take the lowest stack slots, in parameter order; spilled named locals go above
  them in reverse declaration order (the first declared gets the highest slot); a spilled compiler temporary (such as
  a repeated global read that mwcc merges into one value) sits just above the parameters. A local that won't take its
  slot by declaration order may need to be a repeated expression instead (BattleCpu::func_ov007_02231e2c: reading
  `data_020d433c.mUnk_95` at both uses rather than through a local).
- **Which parameters are copied to the stack first.** A stack parameter's copy lands in the early group of the
  prologue only if the parameter is referenced before the first branch of the entry block; the rest are copied
  later, in parameter order. Moving an `if` up splits the entry block (TextManager::Print).
- **A dead store can be load-bearing.** In BattleCpu::func_ov007_02231e2c, `spin = 30;` before a switch whose cases
  all overwrite `spin` keeps 30 in spin's register for later `MATH_Rand32(..., 30)` calls; `spin = 0` breaks it.
- **A `mov` where a spill reload was expected.** When a spilled value's register is still intact at its next use in
  the same block, mwcc replaces the reload with a `mov` (or drops it). So `mov rX, rY` next to a spill store, where
  the original has `ldr rX, [sp, #n]` (or the reverse), means the defining register was or wasn't reused before that
  use. How the variable is built moves this: `s16 x = base; x += off;` versus `s16 x = off + base;`, and statement
  order (BattleStatusPanels::func_ov007_02235480).
- **Narrowing shared between uses.** `(s16) (y - 8)` passed to two calls narrows once and keeps the narrowed value;
  the original kept `y - 8` and narrowed at each call. `(s16) (int) (y - 8)` stops the sharing (a placeholder form).
- **`Screen(n)` versus `mScreens[n]`.** `SpriteScreen &screen = SpriteManager::Get().Screen(1);` goes through an inline,
  so the reference is a compiler temporary (coloured before named locals); `SpriteManager::Get().mScreens[1]` is
  coloured like a named local. Notes like "screen is r7, original r5" (BattleHud) come from this.
- **Named pointer locals change what survives calls.** A local like `ShootPacket *pkt = (ShootPacket *) &g;` keeps
  its value (or a shared base such as `this + 0x6000`) in a callee-saved register across calls, where the original
  writes through the cast global each time and recomputes the base after every call (BattleShoot::Update). Likewise a
  struct-typed table indexed directly, and a field read through `Get().mField`, are temporaries where a named local is
  not (BattleObj00::Update).
- **Enum-typed locals are not constant-propagated.** A known constant the original keeps in a register and computes
  with (`mov r2, #0; mov r1, #52; smulbb r4, r2, r1` for `array[0]`, then `this + r4` re-added at every access) comes
  from an enum local: `UnkStruct_02065c40_Kind j = UNK_02065C40_KIND_0;` matched func_0206d3b8 where `s16`/`int`
  locals, casts, loops and inline parameters all fold.
- **How mwcc picks callee-saved registers (lab results, `int`-sized values live across calls).**
  - Named locals loaded from memory (or assigned later) are coloured in declaration order: the first declared gets
    the lowest free register.
  - Values that come from a call (`int a = g();`, or `a = g();`) and loop invariants the compiler hoists are
    coloured as compiler temporaries: before the named locals, in reverse order of creation (the first hoisted
    invariant or the first call result gets the highest of them).
  - Use counts make no difference; the shape of the live ranges does (where a value is last used).
  - Types and casts matter in combination with order: TournamentScene::Update matched only with `u32 id = (u8) x`
    (not `u8 id`) *and* a particular declaration order. Search order and type together:
    `tools/permute.py` takes `//PERM T=u8|u32|int`, and `tools/auto_permute.py <unit>` tries every order of each
    run of declarations in every non-matching function of a unit (before 2026-10-13 permute.py could not report a
    match at all, so older "every order tried" notes may be wrong).

- **Const-ness of extern tables changes scheduling.** Loads from `const` data may move above stores, loads from
  non-const data may not: `extern const int data_0209e424[]` (AHX bit reader) and `extern const u16
  data_0208f73c[][3]` (GX_BeginLoadTex) each turned a near-miss into a match. Read-only tables should be declared const.
- **Volatile fields and parameters.** A field or by-value parameter that is loaded again where mwcc would reuse or
  hoist it is volatile in the original (`OS_GetTick`'s counter, the FS file flags, `func_0200aa24`'s FSFileID).
- **A constant stored to a member and read back** stays a run-time computation (`mUnk_38 = 0x20;
  mUnk_38 = (0x6c - mUnk_38) / 2 + 0x1e;` in MapScene::ShowNumbers); from a local it folds.
- **Loops whose entry test sits inside the body** (the back edge jumps to a `count != 0` guard) are do-while loops with
  the guard as an `if` inside; the gimmick overlap test also never resets its inner counter, which plain `for` loops
  cannot express.
- **`s16` locals colour differently from `s32` ones** in otherwise identical code (func_02088b30).

- **Narrow reads of stack arguments.** Some callees read a stack-passed argument with `ldrb`/`ldrh` while every
  caller stores it as a full word, so a `u8`/`u16` parameter type breaks the callers (they add an `and`). Until the
  real source form is known, these read the parameter as `*(u8 *) &param` (CellAnimation, MessageWindow); treat
  that as a placeholder, not as the original code.
- **Soft float.** `float` arithmetic calls MSL helpers: `_fadd` 0x0201e5c0, `_fsub` 0x0201f4f0, `_fmul` 0x0201f304,
  `_fdiv` 0x0201edc4, `_fflt`/`_ffltu` (int to float) 0x0201f274/0x0201f2bc, `_ffix`/`_ffixu` (float to int)
  0x0201f200/0x0201f234, and compares `_fgeq` `_fgr` `_fleq` `_fls` `_feq` `_fneq` (0x0201eb6c..0x0201ed50). A
  compare in source must use the same operator as the original (`a < b` calls `_fls`, `!(a >= b)` calls `_fgeq`).
- **Unfolded int-to-float.** `s->x = 8;` on a float folds to a constant; `u32 left = 8; s->x = left;` keeps the
  original's `mov r0, #8; bl _ffltu`.
- **Inlined SDK setters.** A 36-byte copy into 0x02211430 followed by calls to 0x020322c0/0x020322f0 is the
  inline `NNS_G3dGlbSetBaseRot`/`Trans`/`Scale`: `MI_Copy36B` (0x01ff95c4) copies the 3x3 rotation.
- **Flag from a condition.** `if (c) { x = 0; } else { x = 1; }` as statements can give the original's
  `moveq`/`movne` order where `x = c ? 0 : 1` gives the reverse (`func_02023708`).
- **Narrow read-modify-write toggles.** `f++; f &= 1;` as two statements on a `u8` field matches where a temporary
  (`u8 t = f + 1; f = t & 1;`) keeps the add in its own register (`UnkStruct_02065c40::Update`).
- **Arguments a callee ignores.** If callers load a register before a call (e.g. `this` into r0) that the callee
  never reads, the callee still takes that parameter: `func_02084e64(UnkStruct_02078e14 *self)` is passed `self`
  although its body ignores it, and declaring it without the parameter makes the caller 8 bytes short.
- **Declare return values.** A function declared `void` that really returns a value changes the caller's
  register allocation: `func_02025b64` returns `offset + size`, and declaring it `u32` (callers ignore the result)
  fixed runs of loads that kept a zero in a callee-saved register.
- **Runtime helpers.** `bl` to `0x0201f970` is `_s32_div_f`, i.e. a `/` in source: the dividend is in r0 and the
  divisor in r1, which often already holds a value the code uses elsewhere (keep it in a local).
- **`RequestScene` copies the context pointer to a local.** `SceneBase::RequestScene` is
  `UnkStruct_SceneContext *context = mContext; context->mList->...->mNextScene = scene;`. Without the local, the
  context pointer and the request chain share one register at the end of `Update` functions.
- **Message text pointers (`MessageFile::GetText`).** `MessageEntry::mOffset` is declared `volatile`. The original
  always reads the offset where the text pointer is formed, so the load sits right after the file pointer, and
  the zero for `Print`'s size outputs lands in a callee-saved register. A plain `u16` lets the load sink to the
  call and changes register allocation for the whole argument setup. With the volatile field, the natural form
  matches:
  ```cpp
  TextManager &text = TextManager::Get();
  s16 height;
  s16 width;
  width             = 0;
  height            = 0;
  MessageFile *file = mMessages;
  text.Print(file->GetText(i), file->GetLength(i), mTextBuffer, ...);
  ```
  When the original loads the text buffer before the file pointer, load it first in source too:
  `void *buffer = mTextBuffers[2]; file = mMessages[2];` (ov002 `0221ddf8`, `02220830`).
  How it was found: a store placed between the offset load and the call gave the original register allocation,
  so the load only had to be pinned in place.

- **Early returns:** `if (x != 0) { x = 0; return TRUE; } return FALSE;` and `if (id < N) { return table[id]; }
  return 0;` give the original's conditional early return (`bxeq`/`bxcs`); the inverted forms predicate instead.
- **Stopping predication:** explicit nested ifs setting a `BOOL` keep a branch that a ternary or a selected `u8`
  would predicate (`BattleCommandList::func_020585e0`).
- **A field set from an argument and then tested:** store first and test the field (`f = g; if (f != 0)`), not
  `if (g & 0xff)` (`func_02058404`, `func_02055764`).
- **Load order through locals:** a pointer copied to a local before a store fixes the order of pool loads
  (`func_02084a10`); fields loaded into named locals before arithmetic fix the order of field loads, then try
  both operand orders (`BattleTarget::func_ov007_0225aa80`).
- **One-word diffs in the literal pool** can be a wrong constant, e.g. dividing by the wrong number
  (`BattleTarget::func_ov007_0225c360` divides by 182, not 240).
- **A stack argument read twice:** a `volatile` parameter reproduces it (`BattleOrderSelect::func_ov007_0224cb20`);
  like `*(u8 *) &param`, treat it as a placeholder for the real source form.
- **Float compared with an int:** keep the float expression in a helper that returns float and cast at the
  compare (`(s32) FramesToFloat(n) < x`); mwcc evaluates the float, then the left side, then `_ffix`.
- **Indexing in place vs a pointer local:** `owner->mSlots[i].x` written inline instead of through a `slot`
  pointer local fixed a register difference (`func_ov007_0226a700`).
- **Loop-invariant expressions** are hoisted out of the outer loop only when bound to a local; write them inline
  in the inner loop's condition to keep them in the loop.
- **Product order:** `a*b + c*d` evaluates the right-hand product first; write the terms in reverse to swap
  (also true for the NitroSDK compiler).
- **Inline zero fill (`__clear`).** A global cleared by `mov r0,#0; mov r1,r0; mov r2,r0; mov r3,r0` and an
  `stm lr!, {r0-r3}` loop is CodeWarrior's `__clear(ptr, size)` intrinsic (the compiler also knows it as
  `__builtin__clear`); `memset(p, 0, n)` compiles to a call and a zero-initialised struct goes through the stack.
  Found in the CRI library (`func_0203e07c` and four more).
- **Division helpers.** `/` and `%` on ints compile to calls to the runtime helper at 0x0201f970 (`_s32_div_f`,
  quotient in r0, remainder in r1). Write the operator; never call the helper by name.
- **Vector writes through a temporary.** Where the original loads every component before storing any (often as
  `ldm`/`stm`), the source filled a local `VecFx32 t` (z, then y, then x) and assigned `*dst = t;`. Field-by-field
  stores to `dst` interleave loads and stores instead. Same for `*vel = *a`: a plain struct copy can give three
  `ldr`/`str` pairs where a filled temporary gives `ldm`/`stm` (BattleTarget).
- **Compound assignment keeps constants out of folding.** `angle += 90;` and `fx *= 2.0f;` compile differently from
  `angle = angle + 90;` / `fx = fx * 2.0f;`: with the compound form mwcc doesn't fold the constant and keeps the
  variable as the first operand of the float multiply (BattleCommandMap, 74% -> 99.6%).
- **Arguments narrowed at the call.** If callers do `lsl #16; asr #16` on an argument of a function that takes `s32`,
  the caller's variable was `s16` (BattleStatusPanels' `CellAnimation::Draw` calls); changing the callee to `s16`
  breaks every other caller.
- **Loops without a pre-test.** If the original enters a loop body without first testing the count (no
  `cmp; bls` guard), the source was `do { ... } while (k < count);`, not a `for` loop.
- **`%` vs the expanded form.** `x % 300` and `x - x / 300 * 300` compile differently (different `smull` use).
- **How mwcc picks registers (from small synthetic test functions).**
  - Named locals that survive as variables are coloured in declaration order (first declared, lowest register);
    parameters the other way round, and after all locals. A single-use local folds into its expression.
  - Under register pressure the value that interferes with more others gets the lower register whatever its
    declaration position, which is why permuting declarations often does nothing.
  - What does move a pair: a named local copy of a field vs repeating the field access (the compiler shares the
    repeated read; only when no real call sits between the uses), the expression's tree shape (which temporary is
    created first), `*q++ = x` vs `*q = x; q++;` (the first moves `q` to the end of the order), and the type of the
    variable (`u8` vs `u32` with explicit `(u8)` wraps).
  - A value that is also returned is forced into r0: `return g = &x;` where a plain store gives the other order.
- **Constants kept in registers (from small synthetic test functions).**
  - The backend reuses a register that already holds a needed constant within an extended basic block: `mov rd, rs`,
    or `add rd, rs, #(K-C)` from a nearby constant. Knowledge passes only into single-predecessor blocks (a join,
    even an if-converted one, clears it; a call doesn't clear callee-saved registers), and only if the constant's
    definition comes first. So `i = 0;` placed after an `if (n > lim) n = lim;` clamp gives `mov r5, r6`, before it
    `mov r5, #0` (BattleResult's text renderer).
  - A variable updated by compound assignment (`x += e`, `x |= e`, or `x = x + e`) keeps its constant start value in
    a register: `int r = 0; r += 0xe0;` gives `mov r2,#0; add r3,r2,#0xe0`. Its other uses still fold.
  - A constant stored to memory (a member) and read back is forwarded late: `n = 8; f(n * 12);` keeps
    `mov #8; mov #12; smulbb`.
  - Mixing signed and unsigned stops constants combining: `t + 0x24 + 0x28c0u` keeps two adds.
- **Globals: declare the size.** A store through any pointer makes mwcc re-read a global declared with a complete
  type (`extern T g;`, `extern T g[1];`) but not one declared `extern T g[];`, which it keeps in a register or hoists.
  So save-data style globals need their real size: `data_020d43f0` is `UnkStruct_020d43f0[1]`, padded to 0xff4.
  The half-size store-scheduling rule (a later load may move above a store only if the store offset is below half
  the object's size) applies to globals too. Loads through a pointer local die at any global store; loads written
  as `g[i].f` don't.
- **Singleton references.** With `T &a = T::Get(); T &b = T::Get();` the address from the second guard block is
  kept in a callee-saved register and serves `a`; uses of `b` reload from the pool. A single reference reloads at
  every use. Through a singleton reference a stored member is not forwarded to a later load, but a store
  immediately followed by a reload of the same member through `this` is (`mData = data; data = mData;` compiles
  with no load; BattleSlot's constructor).
- **Equivalent arithmetic, different schedules.** Inside a long sum, `y % 8 << 3` and `y % 8 * 8` (or
  `(y/64) * 8 * 512` and `(y/64) * 4096`) produce different instruction orders, and so different registers, even
  though each term alone compiles the same. Term order doesn't matter; the compiler re-sorts the sum
  (SpriteScreen::func_02028054).
- **Single-assignment locals are substituted.** `T x = expr;` is copied into every use and then hoisted out of every
  enclosing loop where it's invariant; passed straight as a call argument it's recomputed instead. A local assigned
  twice (`s32 left = c; left += b;`) is not substituted and is computed where written, e.g. in an inner loop's
  preheader (BattleCommandPage::func_020571a8). Function-scope stack locals get slots in descending declaration order.
- **More register rules (game code).**
  - A value returned from an inline helper (or a block-local copied into the variable) is coloured differently from
    assigning the variable directly; a variable left unset on some path is live from entry and gets low registers.
  - Declaration order sets registers, assignment order sets load order: declare `a, b, c, d` and assign `b, a, d, c`.
  - Several zero-initialised locals share one `mov rN, #0`, put in whichever is assigned first; the later ones become
    `mov rM, rN`. Use declare-then-assign to choose the first.
  - A global read repeated inside a loop is hoisted and coloured after named locals and loop temps, so it gets the
    higher registers; a named local copy is coloured early.
  - `T a[2] = {0, 0};` stores through a base register (`add r1, sp, #N`), separate element stores use sp offsets.
- **Spills, split variables, post-increments.**
  - The values the original spills are the ones declared first; the first declared gets the highest stack slot.
  - A local reassigned to an unrelated value that lands in another register was two variables in the source.
  - Write post-increments as separate statements on the pointer they move (`*base = *r; base++; r++;`); folding
    them into the expression (`*p++`) changes the colouring.
  - Under register pressure, initialisation order can swap a pair when declaration order can't.
  - A parameter copied into r1-r3 at entry and kept until a later call is usually that call's argument in that
    position: check the callee or format string (a missing `%d` argument looked like a register problem).
  - Declaration order doesn't affect `double` locals.
- **Post-increment reads schedule loads.** `s += *w++ * p[x];` per tap keeps each address `add` next to its load;
  `s += w[2*k] * p[x]` with `w += 16;` afterwards gives the same instructions but the scheduler groups the address
  adds (CRI's 16 windowing functions). Indexing through a pointer-to-array type (`int (*scf)[3][32]`,
  `scf[ch][i][sb]`) recomputes `scf + sb*4` each iteration where a running pointer is strength-reduced; chained
  stores `a = b = c = k;` evaluate right to left.
- **`const` on read-only data.** Externs that live in .rodata must be declared `const` (pointer tables
  `const char *const`): mwcc then moves their loads above stores and stack-argument writes, as the original does.
- **Return types show in the callers.** An enum return gives `movlo #0; movhs #1` at the last compare where BOOL
  gives the reverse; a BOOL stored into a u8 field gives `ands rX, r0, #0xff` before the store; a u8 return removes
  the caller's `and #0xff`.
- **Chained assignments** (`a = b = c = 1`) store right to left and change which register holds the constant.
- **0/1 selects always put the 1 first.** mwcc rewrites an int/bool select between exactly 0 and 1 (`x ? 1 : 0`,
  `x != 0`, `!!x`, if/else assigning 0 and 1 to a local, inline `return x != 0;`) into a compare value emitted as
  `movCC #1; movNCC #0`, whatever the source order. The original's `#0`-first pair comes from: a select of two
  enum constants (`x == 0 ? SCREEN_MAIN : SCREEN_SUB`); if/else storing straight to memory in both arms
  (`if (c) p->f = 0; else p->f = 1;`); a variable with other definitions; or constants other than {0, 1}.
  `b = 0; if (x) b = 1;` gives `mov #0; movne #1` instead.
- **More from the library passes.**
  - `__memcpy(dst, src, n)` is a CodeWarrior intrinsic like `__clear`: it inlines to `ldrb`/`strb` pairs (source
    address in r3), where `memcpy` and struct copies call or use other registers (MSL printf's inf/nan strings).
  - A leftover `mov rX, rX` means a store followed by a read-back of the same field (`g.mode = m; ...;
    if (g.mode <= 1)`).
  - Identical trailing statements in both arms of an if/else are merged into the join; write them inside both arms
    when the original keeps them there.
  - By-value struct parameters: use and modify the fields in place (`spec.prec -= 2`) rather than copying them to
    locals; the copies change spill slots.
  - `T *p = o->a; p += o->b;` pins the computation where it's written (not substituted into a call argument).
  - A `double` parameter copied to a local puts its argument loads at entry, above an early return.
  - A named `char *q = (char *)&x` gives indexed `q[i]` loads; the cast expression `((char *)&x)[i]` is
    strength-reduced to a running pointer.
- **Rules from the BattleTarget passes.**
  - Three u16s copied as one object (`*(Rot3 *) rot = *(Rot3 *) p->mRot;` with `struct Rot3 { u16 r[3]; }`) load all
    three before storing and keep the copy in memory; element-wise copies interleave.
  - Guards that branch to one shared `mov r0,#0` exit are nested ifs with a single `return 0` at the end; early
    returns become predicated `movne/popne`.
  - Inline setters (`SetVec(&v, x, y, z)`) compute all values before storing; identical setter calls in both arms
    of an if/else are merged after the join.
  - A struct copy declared inside a loop block (`VecFx32 d1 = *dir;`) stays an `ldm`/`stm` copy re-read from the
    stack; at function scope it's decomposed into registers.
  - An index the original masks with `and #0xff` at each use was not a u8 local: write `owner - 1` at each use.
- **Small-type and comparison rules (band pass).**
  - In a function returning `u8`, `u8 r = c >= x; return r;` has no final `and #0xff`; a BOOL local does.
  - A spilled `u8` local holding a call result is reloaded before each compare of a switch chain; an `int` isn't.
  - `x == 0 || x == 2 || x == 3` folds into a range check; the original's `cmp #0; cmpne #2; cmpne #3` came from
    `switch (x) { case 0: case 2: case 3: ... }`. Write `||` chains in the order the original tests.
  - Compares on an unsigned bitfield are unsigned (`bcc`); `(s32) bf >= 2` gives `blt`.
  - One function-scope loop variable reused by several loops colours differently from per-loop variables.
- **Where a pointer local is assigned.** `T *xb;` declared with the other locals and assigned (`xb = (T *)&x;`)
  just before a loop puts the address computation before the loop guard; `((T *)&x)[k]` at each use moves it after
  the guard, and initialising it in the declaration recolours the whole function (MSL printf's float formatter).
  Index a table with the global field in place (`&tbl[g.a]`) rather than through a named copy of the index.
- **A cast on the address swaps address and value registers.** In a read-modify-write of a global, a direct access
  (`g.x |= 1`, `fl[1] &= ~0x20`) colours the address first (lower register); going through a pointer cast, even a
  no-op one (`*(u8 *)&g |= 1`, `((u8 *)fl)[1] &= ...`, or a helper taking `void *` and casting back), gives the value
  the lower register. The cast also stops reuse of an already loaded base: `*(u8 *)&data_020d43f0[0].mUnk_24 |= 1`
  gets its own pool word. Don't change the shared SetFlagBit/TestFlagBit helpers: existing matches depend on them.
- **The register of a zero stack argument can depend on the calls before it** (a 7-call cycle with
  `func_02025b64(1, 8, 0, ...)` in tests). func_02072734 matched by passing an enum-typed zero local
  (`LoadArg z = LOAD_ARG_NONE;`) for the three zero arguments. The same trick fixes one call in
  BattleUnk022767c8::func_ov007_02277514 but moves the reorder to calls 9 and 10, so that one is still open (2 words:
  the string's `ldr r3` between the second and third zero stack stores).
- **Narrowing changes scheduling, not just instructions.** `-(u8) (mFade / 4)` (or a `u8 level` local) and
  `-((u32) mFade >> 2)` emit the same `and`/`lsr`/`rsb`, but the `u8` form schedules the `rsb` before the next
  literal-pool load (BattleUnk022767c8::Update).
- **More from the 90%-band passes.**
  - A register-form `and` with a pool-loaded mask comes from division plus u16 truncation:
    `u16 v = (x & 0x1fff) / 0x200;` (a shift pair would fold).
  - On u16 values `/ 32` emits `lsr`, `>> 5` emits `asr` (promotion to int).
  - Compound-assigned locals (`s32 q = a; q /= b;`) fix the order of division-helper calls; one big expression
    lets mwcc reorder them. An accumulated `pos += ...` keeps adds left to right.
  - Narrow stack parameters (`s16 x`) need no `*(s16 *)&x` placeholders.
  - `a[n++] = 0;` computes n+1 before the store (counter-example to the separate post-increment rule).
- **NitroSDK matrix concat reuses a column between rows.** Load the last column used in a row into locals
  (`xx = b->_[2]; yy = b->_[5]; zz = b->_[8];`) and reuse them for the first element of the next row, alternating
  the column order row by row. A product of two locals gives umull+mla with sign words on the stack, a local times a
  fresh load gives smull/smlal: both forms in one function are the sign. `(fx32)((e64 >> 12) + t)` gives `adds`.
- **mwcc predicates the else arm:** in an if/else whose predicated arm branches over the other, the predicated arm
  is the source's else. Duplicated call tails in both arms are not merged: write them in both arms.
- **From the BattleTarget/BattlePlayer under-90% pass.**
  - A struct copy initialised in its declaration (`VecFx32 pos = p->mPosition;`) stays an ldm/stm copy; assigned
    later it is split into fields. Write-backs through inline setters load the owner pointer once.
  - Compute all sums before storing (`u16 y = o->mRotY + r[1]; ...; o->mRotX = x; ...`); `o->mRotY += r[1]`
    interleaves loads and stores.
  - A signed compare (`bgt`) on an `ldrh` value means `int v = field; if (v > K)`; comparing the field gives `bhi`.
  - Predicated `movCC/strCC` right after a `bCC` on the same condition is a dead re-test inside the branch
    (`if (x <= K) { if (x > K) x = 0; ... }`): the original has it, so keep it.
  - `f = c != 0 ? tbl[i] : g;` branches; `if (c == 0) f = g; else f = tbl[i];` predicates both arms. A two-case
    `switch` gives a beq chain with the case-0 block out of line where if/else-if predicates it.
  - A struct member at the head of an array element (`mRecords[k].mHead = ...`) folds the element base into the
    offset; `*(T *)&mRecords[k] = ...` hoists it.
- **Bool stores:** `if (x) { f = 1; } else { f = 0; }` computes into the result register
  and stores before the return-value move. `f = x != 0;`, `?:` and `!!` don't.
- **Bool-returning inline getters:** `bool r = false; if (cond) r = true; return r;`
  matches where `return cond;` doesn't.
- **Condition direction:** `mode == 0 ? a : b` if-converts (predicated loads) where
  `mode != 0 ? b : a` branches. `x == 0 ? 3 : 2` vs `x ? 2 : 3` swaps `moveq`/`movne` order.
- **Separate statics vs struct:** several file-level `static u8` variables are addressed
  from one `.bss` base, just like a struct, but schedule differently. Declaration order sets
  their offsets under `-ipa file`.
- **Volatile:** a store-then-reload of the same byte (`strb; ldrb`) means the variable is volatile.
- **Local declaration order drives register assignment** in register-heavy functions.
  Try permutations before restructuring logic.
- **Signedness:** `mvn rX, #0` stored to a byte means the field is signed (`s8` = -1).
  `ldrsb` on a string walk means plain `char` (`-char signed`).
- **Singletons:** `if (!(guard & 1)) { ctor; guard |= 1; }` is a function-local static in an
  inline accessor. The guard and object are shared across modules (`-ipa file`), so they
  must stay one definition. `__register_global_object` (`func_020202ec`) appears when the
  object has a destructor; its chain node lives in the calling file's `.bss`.
- **Element access through accessors:** arrays inside singletons are often reached via an
  accessor method (`Elem(i)`), not by indexing directly.
- **NitroSDK inlines:** write them as the API's inline functions and macros (`FX_Mul`, `FX_F32_TO_FX32`,
  `MATH_Rand32`, `FS_GetLength`, `OS_SetIrqCheckFlag`, `G2S_SetBG1Priority`). Their
  multi-evaluation and casts are visible in the codegen.
- **`new T(args)`** emits a null check around the constructor call; virtual `delete p`
  calls vtable slot 1 (deleting destructor) with no explicit null check needed.
