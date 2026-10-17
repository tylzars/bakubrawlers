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
   - `python3 tools/sync_linked_symbols.py --write`, then `ninja` and `ninja check`: inside complete units,
     symbols.txt takes the linked binary's names (`@N` literals, statics marked `local`) and binding (`weak` inline
     functions), and names inside a linked object fold into it. A name the check already accepts is kept (`@N`
     matches any `@M`), so renumbering after a source edit needs no resync. `ninja check` then reports three
     mismatches that config can't remove: `@1200` (main's destructor node: dsd can't delink GameMain.cpp's
     reference to it if it is local, and main lives in Game.cpp's translation unit) and `data_0201ee78` /
     `data_0201feb0` (the reciprocal tables inside `_fdiv`/`_ddiv`; asm labels emit no symbol).

A function that doesn't match yet is kept in source with a `// non-matching` comment describing the difference.

Matching masks relocated words, so a function can score 100% while referring to the wrong object or the wrong
field of one (`&banks[9]` where the original has `&banks[6]`). `tools/check_calls.py` covers calls only; the link
catches the rest, because the ROM no longer matches. When a fully matched unit breaks the ROM on linking, diff the
linked `build/usa/build/arm9.bin` against `extract/usa/arm9/arm9.bin` and read the differing pool words
(gx_bank's two sub-screen resets were found this way).

### Watching the compiler's backend

When the source shape looks right but registers or instruction order still differ, `tools/mwcc_trace.py` shows what
the 2.0/sp2p2 backend does with one function. It runs the stock compiler under gdb (nothing is patched) and prints
the function's instruction lists at chosen points: after selection, at each coalescing loop, before and after
register colouring, and the final list. `=` marks a definition, `vN` a virtual register, `rN` a physical one,
`[copy]` a register copy, and `--merges` logs each coalescer merge (`keep vA drop vB`).

    tools/mwcc_trace.py my_variant.cpp func_ov007_0226e884 --flags-from src/07_Battle/Battle/BattleShoot.cpp \
        --at coalesce --at color --at after-color --merges > trace.txt

Diff the traces of two variants to find the first stage where they part. Facts read from it so far:
- The coalescer is greedy: blocks and instructions in order, each copy whose two sides don't interfere is folded
  into the lower-numbered register and deleted. Variables get their numbers before temporaries, in declaration
  order.
- When a variable is spilled, a use that directly follows its definition reuses the defining register
  (`mov r5,#0; str r5,[sp,#n]`). A use with any instruction in between reloads it from the stack, and the later
  local clean-up turns that store-then-load into a register copy (`mov r0,#0; str r0,[sp,#n]; mov r5,r0`).
- Loop invariants hoisted by the backend go into a block of their own just before the loop, after the code of
  the block that precedes it.
- Colouring takes named locals from the highest virtual-register number down, each getting the lowest free
  register. A variable assigned in both arms of an if/else gets a low number and so is coloured last; a short-lived
  value per arm gets a high one (RankingCharacterView::CreateCharacterModel).
- A `mov rX, rY` just before a load through rX, with rY still live, can be a second identical load that the
  post-allocation clean-up turned into a copy. A read through a `const T *` (e.g. a const-parameter inline helper)
  keeps that second load apart in the optimizer (CharacterModel::SetExtraAnimEnabled, StoryCharacterModel::SetExtraAnimEnabled).
- Before colouring, a scheduler orders each block. Once live values reach the register limit it picks by a
  pressure score (start 16; each last use subtracts 1 + max(1, live - limit), each new value adds max(1, live -
  limit)), keeping source order on ties. Colouring then goes in falling virtual-register order, high-degree nodes
  first.
- Colouring order in detail (`--simplify`): scans run over the virtual registers in increasing number; each one
  with fewer conflicts than the register count is removed and pushed, and scans repeat until none is left (a stuck
  one goes by lowest spill cost / conflicts). Registers are then handed out popping the stack, so the last pushed
  picks first and takes the lowest free register. A rotation of callee-saved registers between a few values means
  they left the graph in a different scan: count their conflicts at the scan where they're visited.
- So between two values pushed in the same scan, the one with the lower virtual-register number is coloured later.
  Named locals are numbered below every temporary. When a short-lived temporary has to share a callee-saved
  register with a value that lives across calls (the original reuses it once the long-lived value is dead), the
  long-lived value must be coloured first, so it needs the higher number: keep it out of a named local. Express it
  through an expression whose result is a temporary and whose arithmetic folds into its one later use. In the
  BattleSlot constructor, `BattleData *d = mData;` (a named local, numbered below the `unk4` byte) let `d` be
  coloured after the byte, so the byte took ip; `BattleEntry *entries = mData->mEntries;` leaves the pointer
  living across the calls as a temporary numbered above the byte (the `+12` folds into its use after the `new`),
  so it is coloured first, takes r5, and the byte then shares r5 as in the original. Routes that try to make the
  short-lived value a named local instead fail when the backend's copy propagation removes the local in a
  straight-line block.
- The schedulers (one before colouring, one after) pick an instruction whose latest start time has arrived first,
  then the one that makes the most successors ready, then the taller one, then list order.
- A u16/s16 local or stack parameter is spilled to a full 32-bit slot. Its spill stores and reloads use the
  worst-case alias set, so they're ordered against each other, and the clean-up after allocation won't reuse a
  register value across them. So a parameter's or local's width can move both scheduling and copies:
  TextManager::Init matched once `height` was u16 (one store-order edge lengthened the critical path by a cycle),
  Print once `maxX` was u16 (it keeps a real `ldrne r0,[sp,#92]` reload) and `startY` s16.
- `--sched` logs only the first scheduling pass.
- A function that really uses an object declared `__attribute__((aligned(N)))` skips the IR optimizer entirely
  (one pass in the dump instead of ~100): loops stay unrotated, no strength reduction, globals are reloaded from the
  pool. A dead reference doesn't trigger it (ctrdg func_02011e7c).
- `#pragma scheduling` does not exist in this compiler; sweeps that set it changed nothing.
- Memory-dependence edges come from alias classes, set by what defines an access's base register: an integer
  constant (a hardware register such as `*(vu16 *)0x04000300`) gets the worst-case class, so it is ordered after
  every earlier global store whatever the volatility or casts; an address taken from a symbol (`lda &sym`) gets that
  symbol's own class and can move past stores to other objects. When the original reads a hardware register ahead
  of an earlier store, declare the register as an extern object and pin its address with an `alias` line in
  config/usa/arm9/lcf_edits.txt (ctrdg func_02011e7c, as wm's 0x027ffc30).
- Switch layout: cases are sorted and merged left to right into jump-table clusters while count/range >= 0.4,
  computed in x87 floating point; the tree's root is item (lo+hi)/2. The comparison is sensitive to the host's x87
  precision when a density is exactly 0.4, which is why the build needs wibo with Windows' 53-bit precision
  (tools/build_wibo.sh); msl_printf_fmt and GX_SetBankForSubBG only match with it.
- The register a popped node takes: the lowest free of r0-r3; else a callee-saved register already handed out; else
  r12, then r14; else a new callee-saved register (traced on BattleSlot's constructor in four states).
- A value assigned inside a loop is loop-variant to the IR, so `s16 sdelta; ... sdelta = delta; shift = sdelta *
  sdelta / 8;` inside the loop keeps the product there for the backend's own invariant pass, where a pre-loop form
  is hoisted by the IR into the preheader and scheduled differently (BattleData::DrawEntryNumber).
- Library files built with 2.0/sp1p5 can be traced with `--sp2p2-stand-in` when their probe shows the same
  difference under 2.0/sp2p2 (true for e_pow and cri_wave_ch).
- The IR keeps an assignment to a local that lives across blocks as its own statement only when the same local is
  assigned again later in that block; otherwise the value is folded into its use. Reusing one variable for two
  arguments (BattleResult::func_ov007_0224f9a0's `y`) is a natural way to pin the first one's evaluation point.
- A call result kept in a local loses its name in the backend's first local CSE: inside the call's extended block
  (the call's block and every following block with a single predecessor) each use of the local is replaced by the
  call's result temporary, the copy into the local becomes dead, and the temporary, numbered after all locals, is
  coloured on its own. The local keeps the value only if one use sits in a block with two predecessors. A repeated
  identical test `a || a` makes such a block for free: the IR keeps both halves (both branch into the same block),
  and after colouring the head/tail merge and the conditional optimisation drop the repeated compare and its branch
  (cri_wave_ch func_02048940). A different-looking second test (`n >= 1` after `n > 0`) keeps its compare.

## Placeholders

Forms that reproduce the original's bytes but are probably not its source. Each is marked in place; a real form
that matches should replace it.

- Layout stand-ins: FieldSelectScene's, BattlePlayer's and BattlePlayerList's (16 bytes) `DataLayoutSlot()` statics (one unidentified small object
  in the data size sort; in BattlePlayer it puts the TextManager, InputManager, WirelessManager destructor nodes in
  the original order), BattleAbility's thirteen named `{0, 0x4800, 0}` statics (the order of identical initialisers),
  TournamentSubScene's `sMessageIds[1][24]` shape (keeps a byte table in the shared .rodata), GameMain.cpp (main's slot).
- Index/register stand-ins: ctrdg func_02011e7c's `reg` pointer local for the 0x04000300 read (keeps the address
  numbered among the variables), nns_snd_wavstrm func_02039a50's mix of the ADPCM_STEP macro (high nibbles) and an
  inline AdpcmStep (low nibbles), which only sets the locals' numbering, TP_CalcCalibrateParam's `(s16)` casts on x0/y0 (possibly not original: they make the
  following range checks unable to fail, though the compiler still emits them), gfd_vram func_02035f54's `data_020a5898` (a second name for the +8 range of the slot
  table at 0x020a5890, so the loop base stays a hoisted temporary), cri_adx_dec's unread zero initialisers in func_0203f714 and func_0203fcd8 (register and
  stack-slot order), BattlePlayerList::Update's two volatile reads in case 4's second record (keep the player
  reload and the slot read apart so they number as the original's registers need), BattleSub48's enum-typed entry index, BattleAbility::func_ov007_02265af8's two
  volatile index reads, BattleOrderSelect's `volatile ry`, BattleCardModel's narrow stack
  read, the casts through `data_020a3210.mOrder` in ListScene.cpp, BattleConfrontCard::func_ov007_0223f918's
  two-arm player index in case 29 (both arms read the same byte), cri_wave_ch func_02048940's repeated
  `r6 > 0 || r6 > 0` test (a join below the get24 call, so its result stays in r6's local).
- Expression stand-ins: nns_snd_wavstrm func_020394c8's `- -v`, msl_strtod's `mp = (unsigned char *)&mant`,
  FileLoader's zero `base`, BattleCardCamera's -0x4000 constant, GameData's `mUnk_460 + 0xa0` cast.
- Judgement calls: CharacterModel::LoadPartPalette is declared void (its body is a tail call and every caller
  ignores the result; void is what keeps LoadPartTextures matching).

## Hard problems and how they were solved

- The GameOverScene/BattleUnionAbility/BattleUnk0227532c "literal order" problem is resolved: GameOverScene's
  tables come before most of its includes; the other two were loading the wrong file names (probes mask string
  addresses, so a wrong literal can look like a match).

- **Two CameraManager mode tests, loads scheduled first (solved).** AntennaIndicator::Update, SetupLayout and
  BattleCardEffect::UpdateAnimation (the last thing keeping BattleCardEffect from linking) test
  `(mode == 2) | (mode == 3)` on two `CameraManager::Get()` results. The original loads both modes before the first
  compare: the `== 2` operand comes from the address kept in r4, the `== 3` one from a fresh pool load issued first.
  `CameraManager &b = Get(); CameraManager &a = Get(); if ((a.mMode == 2) | (b.mMode == 3))` is 4 words off (wrong
  pairing, right order); swapping the pairing gives the right registers but compares before the second load. About 60
  forms tried (temporaries, inline helpers, `||`, negations, pointer locals), and `-proc generic` shows our compare
  directly after its load in all of them. Solved with enum-typed copies of the two modes, which are not
  constant-propagated, so both loads stay ahead of the compares.
- **A u8 index coloured after its base.** In BattleTarget::func_ov007_0225ca48, BattleAbility (func_ov007_022677cc,
  02264a5c, 02265af8, 0226547c) and BattlePlayerList (Update, 022628e0) the original colours a u8 index load
  (`unit->mUnk_0e`, a record or row index) after the base or player pointer it indexes; every plain form colours it
  first. An enum-typed copy (`UnitIndex e = (UnitIndex) unit->mUnk_0e;`) or one index local reassigned for
  successive records gives the original's order. Only BattleAbility::func_ov007_02265af8 still needs two
  `*(volatile u8 *)` placeholders: there the enum copy gets the right registers but is scheduled before the list load.
  BattleConfrontCard::func_ov007_0223f918 (case 29's loop head) is the same family: the original numbers the list
  pointer, the player index, `j*4` and the slot pointer in that order. It matches with a placeholder two-arm index
  (see Placeholders): `idx = count ? list->mUnk_2a9 : data_020d433c.mBattlePlayers->mUnk_2a9` keeps the index a
  statement of its own until after the last expression propagation, and the merged arms leave no code.
- **Game.cpp (linked).** What it took:
  - `main` (0x02000c04) is Game's but placed right after the start-up code: a stub unit GameMain.cpp holds its slot and
    `place GameMain.o main Game.o` in config/usa/arm9/lcf_edits.txt puts `OBJECT(main, Game.o)` there;
    tools/objdiff_place.py scores it against Game.o.
  - Game.o's copies of the singletons' guards (0x020c17a8-0x020c17e0) and instances (0x020c1870-0x020d42e8) are the
    kept ones, so its `.bss` range covers them; `inner <instance> <size>` lines alias the field names other units
    use (data_020c1a4c, data_020c3938, ...).
  - The singleton destructors are `inline` in Game.cpp, which gives the original function order.
  - Six singleton classes have an empty constructor as Game.cpp saw them (guard before node there, node before guard
    in every other unit): the headers declare it under `#ifdef GAME_SINGLETON_CTORS`, defined only in Game.cpp. An
    exhaustive search over all 14! orders found no layout without it, and declaring it everywhere breaks 32 units.
  - gLanguageSuffixes (the language-letter table) and its strings are Game's `.data`; SaveManager is one byte,
    SceneRunner 0x14 and FrameTask 0x96c (sizes read from Game's `.bss`).
- **Two literal-pool entries for one address.** `LoadFile` (FileSystem.cpp) loads 0x020d53e4 from two
  different pool words, so two relocations (different symbols, or symbol plus offset) resolve to the same address
  in the original. The source models it with a second extern, `data_020d53e4_h`; the real layout of the loader's
  statics (header word, registration node, two MI contexts, state fields) is still to be worked out.
- **Order of identical string literals (solved).** Copies of one literal in a table's initializer are ordinary
  participants of the file's size sort (below), so their order follows from the whole list. `tools/layout_search.py`
  models that sort and finds the include and definition orders that reproduce the original (ArenaScene's five window
  paths, SaveWindow's four).
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
  section. Pool order in LoadGraphics came from building the pointer inside the loop
  (`const u8 *ids = t.ids; ids += i;`).
- **Non-const tables after a file's strings (CharacterEditScene).** Two writable cell tables sit after the 24 strings
  of the item tables' initializers, not right behind the vtable; as function-local statics of inline accessors
  (`inline WindowCellRecord *CellTable_020b3a44() { static WindowCellRecord t[7] = {...}; return t; }`) each gets its
  own `.data` section there. A table read only with constant indices (camera rows) was kept referenced by reading
  it in the code (`FxRound(t[r][k] * 0x1000)` folds to the same immediates).
- **Vtable order and destructor order can disagree (FileLoader).** Vtables come out in reverse class order, and
  inline virtual destructors are emitted with them (D0 then D1). The original has the two vtables in one order and
  the destructors in the other, with D1 before D0: the destructors were defined out of line after the other
  functions (`SaveWriter::~SaveWriter() {}`), which emits D1, D0, D2 (the linker drops D2).
- **A singleton node at two addresses splits a file (MessageScreen).** The MessageScreen constructor's
  TextManager node is at 0x02219308, while UnkStruct_02071d24's functions use one at 0x02219314, and the
  constructor's strings sit between the two classes' vtables. Split at 0x02071d24, both link.
- **A shared section can't put a bigger object first.** BattleTarget's `.rodata` has a 10-byte table before an 8-byte
  one, which the size sort never produces from separate tables: the block 0x0227a5e0-0x0227a74c is one struct
  (`TargetTables`, members named by original address), as in MyroomGiftScene. Byte tables indexed by a variable stay
  separate (inside a struct the indexing becomes an immediate offset).
- **One class, two names.** SaveWindow.cpp constructed FileLoader's `SaveWriter` under the name
  `UnkStruct_020a54a0` (its vtable symbol's address), which only showed as an undefined vtable once FileLoader linked.
- **The main module's code ends at 0x0208b4bc.** dsd had put the `.text`/`.rodata` boundary at 0x0209d490, so ~70 KB
  of constant data (the SDK's FX tables, sndVolumeTable, GameData's and math_sha1's tables) counted as code and no
  unit could own it. With `.rodata` starting at 0x0208b4bc math_sha1 links and the progress numbers stop counting
  those tables as code. A few hand-written routines inside that block (func_02090b24, func_02091768, ...) are now in
  `.rodata`; they still disassemble and link.
- **Big globals with their own section (GameData).** data_020d433c and gSaveData are defined in GameData.cpp
  inside `#pragma section npdata begin`/`end`: in the shared `.bss` the code would address them from the section
  base and stop matching. Other units still name fields inside them by address (gCharacterLook ...);
  `inner gSaveData 0xff4` in config/usa/arm9/lcf_edits.txt defines every such symbols.txt name as base + offset.
  Linker-script aliases go after the sections, where the base symbols have addresses (before them they resolve to 0
  without an error).
- **False relocations in data blobs.** dsd reads any word that looks like an address as a pointer: a font bitmap
  in main's `.data` at 0x020a8d10 held the bytes of 0x02220842, recorded as a relocation into ov005. Once MapScene
  linked from source that symbol was gone and the word became 0. Deleting the line from relocs.txt fixed it; check
  relocations into a newly linked range from other modules when the ROM differs by a single word.
- **Function order is definition order.** g3d_res matched but its ROM differed once linked: `func_02033788` was
  defined before its caller `func_02033714` (for the declaration), so mwcc emitted it first. A prototype and the
  definition after the caller fixed it.
- **Literals before a vtable mean an earlier file (MessageWindow).** Function literals are emitted after a file's
  shared `.data` section, so CharacterIcons' six path strings lying before MessageWindow's vtable showed the
  two classes were separate files; split at 0x0206550c, both link.
- **Many statics: `layout_search.py --climb N`.** Permuting more than about 7 statics with 5 singletons is too slow;
  `--climb 300` hill-climbs singleton order, static order and the split instead. AttrSelScene (8 tables, 5
  singletons) linked with its first answer: TextManager, TouchManager, SpriteManager and InputManager headers, then the
  tables, then SoundManager's.
  BattleResult (8 tables, 7 singletons) the same way: SoundManager, SpriteManager and InputManager headers, the tables,
  then WirelessManager, TouchManager, FrameTask and TextManager; its `.rodata`, `.data` string and `.bss` node orders
  all follow from that one list.
- **Statics between includes (UnkStruct_02076058).** Defining its three window tables reordered the four
  singleton nodes in `.bss`. `layout_search.py` lists working orders with a cost (how many things move, not how many
  objects are off); the one used defines the tables right after the first header (TouchManager), with
  `UI/SelectWindow.hpp` included early for the record types since it pulls in no singletons.
- **BattleCharaAct's data layout (solved).** The 0x28-byte light rows at 0x0227af68 are a 1-D `u8[8 * 0x28]`: as
  `u8[][0x28]` they stayed in the shared `.rodata` and came out before the 17 `u8[16]` rows. A 1-D char array gets its
  own section even when referenced, a 2-D one stays in the shared section, and own sections are emitted at their place
  in the size sort, after the shared one. With that, the five singleton headers at the end of the includes and one
  definition order of the 39 tables found by an offline model give the original in all three
  sections. Also: FrameTask's singleton objects join guard first, which a compile where both orders fit can't tell.
- **Guard load before a preceding store (solved).** mwcc lets a following load (e.g. the next singleton's guard)
  move above a store only when the store's offset is below half of the declared size of the object stored into.
  `TextManager` was declared 0x5c bytes, so the `mUse4bppFont` store (offset 0x5b) blocked it; with its real size,
  0xe8 (an unused trailing member, so `Widths()` keeps its code), 22 functions match. So when a store seems to
  pin a later load, check the declared size of the stored-to object.

- **Both CameraManager mode loads before the compares.** `(Get().mMode == 2) | (Get().mMode == 3)`
  (BattleCardEffect::UpdateAnimation, AntennaIndicator::Update and SetupLayout): the original loads the
  instance address, then both modes, then compares (first mode and its 0/1 result share r1); we compare the first
  mode before loading the second. Every form tried compiles to the same code: operand order, `||`, negations,
  temporaries before or after the second `Get()`, references, pointers, inline wrappers around `Get()` or the
  compares, and `mMode` as volatile, unsigned or at another offset. No other 2.0 build differs.
- **objdiff and calls inside a unit.** A `bl` to another function of the same unit counts as a mismatch while that
  callee does not match, even though name and addend agree; such a caller reaches 100% once the callee does.
- **objdiff and a call to the next function.** A `bl` to the function that starts right where the caller ends stays
  a mismatch (99.99%) even when everything matches: in the original's object, symbol plus the -8 call addend lands
  in the caller's own last 8 bytes, so objdiff reads it as a branch inside the caller. Linking is the proof for
  these: once the unit is `complete`, the ROM's SHA-1 covers the call. (BattleCommand::Update, BattleObj00::Update,
  BattleResult::Update, BattleCharaAct::Update, BattleCardSpec::Update, ArenaWindowScene::Update,
  ArenaScene::func_ov001_022197ec, func_01ff9b64, ListScene::Update.)
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
  from allocation itself. BattleResult::func_ov007_0224f9a0's last 5 words were a scheduling difference: the
  original finishes the third Draw's y before loading `this`. Reusing one `y` variable for the third and fourth Draw
  calls reproduces it.

## Source patterns that matter

- **Separate globals, not a struct.** UploadQueue's count/head/free/tail bytes and byte total only match as five
  globals (defined in the order that gives the original offsets): as one struct, the entry accesses and the state
  stores reorder across each other.
- **A zero-initialised table that the original has in `.data`** needs `#pragma explicit_zero_data on` around it
  (UploadQueue's gPendingFrees); `= {0}` alone puts it in `.bss`.
- **A table offset as its own variable before the bounds check** (`u32 pos = id * 0x20; if (pos >= size) ...`) gives
  the separate shift the original has (fs_overlay, fs_archive).

- **`x + x + x + y` for a times-three blend.** The G3D animation blends (func_0203382c, func_01ff9f18, func_01ffa1cc)
  match only with the sum spelled out (`(s64) x + x + x + y` for the 32-bit arm); `x * 3 + y` creates temporaries in
  another order and swaps the `adds` operands.
- **Advance the pointer, then read past it.** `p += idx * 2; y = *(s16 *)(p + 2);` gives a different register pair
  than a second `p + idx * 2 + 2` expression.

- **A dead product still shapes the code.** GimmickUnk_02274830::Update copies a vector whole and scales all three
  components (`v.y = FX_Mul(v.y, 0x2000);`) before overwriting y; dropping the dead product changes registers and
  order (BattleUnk29c and GimmickExePoint do the same).

- **A guarded allocation that calls a constructor returning `this` is `new T`** (Mobiclip's Player: operator new,
  operator delete, constructor and destructor are members); the new-expression's result is a temporary, which fixed a
  register swap.
- **`else { return x; }` keeps a branch** where `if (!c) return x;` predicates the return (MobiclipFile's allocators).
- **One function-scope pointer reused in both branches** of an if/else changes how the second value is coloured
  (FileSystem's LoadFile).

- **Create the first operand early, add the second late:** `u16 rx = p->mRotX; ...other sums...; rx += r[0];`
  instead of `u16 rx = p->mRotX + r[0];` (which is substituted into its store and created last) gave BattleUnk29c's
  four rotation tails the original's reverse-creation register order.
- **A pointer taken from a member just after storing it** (`mUnk_40 = data; entries = mUnk_40->mEntries;`) is
  scheduled where `data->mEntries` is not (BattleSub48's constructor).

- **A named local loaded from a const table can take a register ahead of an earlier local;** reading the table at
  each use (the reads are merged, even across calls) gives the original order (ScriptPlayer::LoadMap).

- **A spilled s16 keeps its reload at an early return.** A spilled `s32` whose register still holds the value at a
  predicated early return has the reload dropped; as `s16` (still stored and loaded as a word) it keeps
  `ldrcs r0, [sp]` (ScriptPlayer::func_02066814).

- **`unsigned long` vs `unsigned int`.** NitroSDK's `u32` is `unsigned long`; this repo's is `unsigned int`.
  CreditsScene::UpdateText's `for (unsigned long i ...)` matched where `u32 i` made a second copy of `this + i`; it
  depends on the loop variable's own type (casting its uses either way changes nothing). A sweep of 39 other
  near-misses (every u32/s32 local, counter and cast as long/unsigned long) helped none: in 32 the code didn't change
  at all, in 7 it got worse. cri_adx_vobj matched with an `unsigned long` counter too. So it is a rare fix for loop
  counters in game code. CRI code is different: `long` locals and parameters matched cri_adx_hdrparse (`long i` turns
  a `mul` into the original's `smulbb`) and fixed cri_mpa_l2's stack frame and cri_wave_ch's temporaries, so try
  `long` early there. It only matters where it makes a mixed-type expression (`v % grp` with `long grp`); on values
  used only with `int` it changed nothing.
- **An enum-typed zero stays in its register.** cri_adx_player's func_0203e110 keeps one zero in r2 through a loop of
  byte stores: `enum { ZERO } z = ZERO;` and `p[k] = (unsigned char) z;` (the cast avoids a sign extension), since
  mwcc does not propagate enum values. Plain `int`/`long` zero locals fold back to constants.
- **A shared exit label.** GimmickFieldDestroy::PickKind's two early `return i;` became `goto found;` to one
  `found: return i;` after `return 0;`, which gives the original branch shape and table load order.

- **Two u8 zeros share a register, and that steers the scheduler.** In BattleCpu::func_ov007_022310f0 the zeros of
  `free` and the counter shared one register, which put `mov r5, #0` ahead of a load; a `u32` counter with a `(u8)`
  wrap stops the sharing and gives the original order.
- **Store, then scale in place:** `d->x = FX_Mul(s->x, len); d->x = FX_Mul(d->x, k);` colours differently from one
  expression through a temporary (func_ov007_022337ec).
- **A pointer declared before a loop and assigned inside it** (`u = &list->Units()[i];`) fixed a base/element
  register swap no declaration order did.

- **A load folded into an address is a temporary.** `rec = &pl->mRecords[pl->mRecord]` makes the index a temporary
  coloured before `pl`; kept as its own statement it is coloured after `pl`, which let identical if/else tails merge
  in BattleTarget::func_ov007_0225ca48. There the separate statement is an enum-typed copy of `pl->mRecord`;
  computing the address in both arms is what puts the shared constant 12 before the branch.

- **A local reused for an earlier, separate value** gets its later live range coloured after the loop counter
  (BattleTarget::func_ov007_0225ac28: one `fx32 value` for the magnitude test and for `f[i]` in the loop).
- **A helper called with a different constant in each arm** of an if/else is compiled per arm and merged into
  `moveq`/`movne` only after the first scheduling pass, so a later call's argument setup is not moved up into the
  products (func_ov007_0225c098: `ScaleVec(&v, 0x14000)` / `ScaleVec(&v, 0x5000)` instead of a selected scale).

- **A declared 2-D member is not the same as a cast to one.** FieldSelectScene's constructor needed `u8
  mOptionBytes[5][2]` (a union view of the flat `mUnk_25c[10]`): a declared 2-D member builds the address as
  `j + row` (`add r2, r9, r3`); `((u8 (*)[2]) mUnk_25c)[i][j]` fixes the registers but emits `row + j`.

- **Locals shared by two blocks colour as named locals.** In BattleSub48::DrawCircleAndSides per-block `u16 x` was a
  call-result temporary (higher register); declaring `x`, `y`, `step` once at function scope and assigning them in
  both blocks matched.
- **A static's place in `.bss` shows in relocations too.** GimmickFieldDestroy::Load/Unload were byte-identical but
  addressed `sResources` at the wrong `.bss` offset; defining it between two tables put it at offset 0, before the
  singleton nodes.

- **An inline's wider parameter can turn a local into a temporary.** Passing MapScene::ShowText's `u16 cols` to
  `TextManager::BufferSize(u32, u32)` created a conversion temporary that merged with `cols`, so it took the
  temporaries' register whatever the declaration order; with `u16` parameters it is a named local again.
- **`p[c]` vs `p += c; *p`:** indexing in place loads the other operand first (NameEntryScene::CheckSpecialName).

- **A pointer local hoists an address early.** Through a pointer local, a flag array's address is hoisted (so placed
  in the pool and coloured) before a table read only in the loop condition; used directly in the expression it is
  hoisted after it. `(u8 *) g[0].member` gets the direct order and keeps aliasing with `g`, which an extern alias for
  the same bytes doesn't (UnlockStoreItems, UnlockProgressEntry).

- **Recompute differences instead of naming them.** BattleShoot::func_ov007_0226f698 matched once `x0 - x1` and
  `y0 - y1` were written out at each use (with an `ABS` macro) instead of held in `dx`/`dy` locals.
- **Where a shift sits matters.** `fx32 distance = f(...) << 12;` then `FX_Mul(distance, k)` matched
  func_ov007_0226dfb8; shifting inside the `FX_Mul` argument did not.
- **A zero shared by two spilled stores keeps its own register.** With one spilled variable zeroed before a loop,
  the loop counter takes over the zero (`mov r5, #0; str r5, [sp]`); when the same zero also feeds a second spilled
  store first, mwcc emits `mov r0, #0; str; str; mov r5, r0`. BattleShoot's e884 shows
  the second pattern with only one visible store, so its other zero consumer is still unknown.

- **A callee declared `void` that returns a value changes scheduling in its callers.** `SpriteScreen::LoadCharacters`
  ends in `return QueueUpload(...)`; declaring it `u32` flipped two scheduling ties (`mov r0, r4` against `ldr r1`)
  in func_020843ac (ListScene.cpp), not only register choices.

- **`x[n] = v; n++;` vs `x[n++] = v;`** schedule differently; BattleCpu::func_ov007_02231300 needs the split form. A
  counter also passed to `MATH_Rand32` matched as `u32 n` with `n = (u8) (n + 1)` (StartJumpObjTargets).
- **Re-read instead of caching.** Where the original reloads a table element or a global chain in a loop, read it at
  each use rather than through a local pointer (func_ov007_02231300, func_ov007_022314bc).

- **`p = e; p += idx;` splits the address CSE.** Built that way, only `idx * size` is shared and the base is re-added
  at each use (`ldrh r0, [r6, r7]`); `&e[idx]` or `e + idx` shares the whole address (BattleConfrontCard::func_ov007_022428d4).
- **Declared before, computed after.** Declaring `opp`/`oppLevel` before `self`/`selfLevel` but computing the self
  chain first changes which value is spilled, independently of either order alone (same function).

- **Zeros from one declaration share a register.** `s32 done = 0, advance = 0, pending = 0;` as one declaration lets
  the three zeros share one register, where separate declarations (or `= done`) don't (ScriptPlayer::func_02066814).
- **`u8 now = ++field;`** gives `add; and; strb` with no reload of the field; `field++; now = field;` reloads it.
- **A chained assignment as an argument:** `SetAnimation(mCutsceneAnimIndex = value)` schedules the store differently from two
  statements (SetCutsceneAnimation).

- **Parameter width shows in the caller.** Passing a `u8` to a `u32` parameter makes a conversion temporary that
  shifts the caller's registers; FieldSelectScene's `PrintSlotName(u8)` fixed two callers' loops.
- **A pointer kept only part of the way is a second `Get()`.** When the original holds a SpriteScreen (or other
  singleton part) in a register through some loops and then reloads it from the pool, the later uses come from a fresh
  `SpriteManager::Get().Screen(n)` (FieldSelectScene::SetupPage: 1082 differing words to 0).
- **A local array filled from a one-use rodata table is an aggregate initializer.** `u8 buf[3] = {0, 1, 4};` makes
  mwcc emit an anonymous rodata object (often first in the unit's .rodata) and changes which temporary is created
  first; copying `buf[i] = data_xxx[i]` element by element does not match (TitleScene::Update, BattleTarget::Update).
- **Check a callee's real parameter type before fighting a caller's registers.** An `s16` local passed to a `u32`
  parameter inside a loop is widened by a hoisted temporary that mwcc coalesces with the local, so declaration order
  stops affecting it. With `AddOam`'s third parameter as `s16` (its prologue then matches too), MyroomGiftScene
  and MyroomShootScene matched by declaration order alone.
- **Repeated calls with constant arguments schedule in a cycle, and an earlier call's return type shifts it.** In a
  lab of 20 identical `LoadAndQueueUpload` calls the argument-setup pattern repeats every 7 calls; the period depends on
  the stack arguments (10, 8, 7, 6, 6 calls for 1-5 of them). Code before the run shifts it: a global store, or a
  call that returns a value (a `void` call shifts it less). The open cases (TournamentSubScene::Init,
  BattleMoneyReward::Init, and the enum-zero placeholders in MyroomGiftScene and UnkStruct_02071d24)
  were all one cause: `func_0200acac` (called by the `ChangeDir` inline) returns BOOL but was declared void.
  Declaring it BOOL matched them; CharacterModel::LoadPartPalette then needed to be void to keep LoadPartTextures matching
  (its body is a tail call and every caller ignores the result, so either type fits the binary).
- **A stack parameter the callee reads as `*(s16 *)&param` is really `s16`.** Declaring it so makes callers narrow at
  the call, which creates the value later and gives it the low register: with
  `SpriteScreen::AddOam(..., s16 unk4, ...)`, `int y = row * 0xc + base;` passed straight through matched
  TournamentScene::Draw. The callee's other narrow reads (`*(u16 *)&unk6` and so on) are untested.
- **A pointer local can hoist shared index arithmetic over a branch.** `BattlePlayer *pl = list->mPlayers[p];`
  makes `p * 4` a common subexpression with a later `p * 4 + 0x28`, and mwcc hoists it above the `if`. Keeping the
  container in a local and indexing inside each arm (`list->mPlayers[p]->mUnk_f1`) stops it
  (BattleCommand::func_ov007_0223a734). A `(u8 *)` cast store recolours differently from a real `u8` field store.
- **An enum-typed result stops the return being folded.** `UnitCompare result` kept a `return result` that a `u32`
  version folded into `mov r0, #0` (BattleAbility's func_ov007_02267884). Reading components into named locals
  in the original's order (`z = p->z; y = p->y; x = p->x;`) fixes the load order before a call.
- **An enum-typed copy of a field read is a real load.** `CameraMode mode = (CameraMode) cam.mMode;` loads where it
  is declared and is not substituted into the compare, so two such copies keep both loads ahead of the tests: this
  matched AntennaIndicator::Update/SetupLayout and BattleCardEffect::UpdateAnimation after ~60 int-typed
  forms failed. Parameter widths matter the same way: BattleMapModel matched once `func_02032370`'s parameters were
  declared `u16` (the conversion temporary had merged with `color`), BattleObj01 once its own `layer` became `u8`.
- **An identical first instruction in both arms is hoisted after register allocation.** When both arms of an
  if/else start with the same instruction (same registers), mwcc moves it above the branch; that is not CSE or code
  motion in the source, so source forms that try to stop a "hoist" cannot work. The original keeps such loads in
  each arm only because the arms load into different registers (CharacterModel::SetExtraAnimEnabled's restart arm uses
  r2 for the part pointer, plus a `mov r2, r0` copy).
- **One more live value across a loop changes the loop body's schedule.** BattleBase::func_ov007_0221c938 matched with
  `const u8 *mode = &data_020c4258;` used by the three IsWirelessMode calls: the pointer is reloaded from the pool,
  so it leaves no visible code, but it changes the schedule before allocation. A block-local pointer also gets a
  different register from a function-scope one (ParkCharacters GetParkEntry's loop pointer). A local reassigned in
  place keeps its register (`cx += width;` before the multiply in func_02027cf4), and `u16 *` indexing keeps an index
  update in place where byte-pointer arithmetic lets it sink past calls (func_02078ab8).
- **Search tool.** tools/permuter.py runs a simulated-annealing search over many source transformations (statement
  order, scopes, enum-typed copies, temporaries, an inline identity, casts, compound forms) with a graded distance and
  stripped compiles; run it in the background on a near-miss.
- **Declaring locals ahead of a call-surviving local can swap `this` with it.** In BattleSlot::BattleSlot, declaring
  `a`, `b` and `base` at function scope before `BattleData *d = mData;` pulls the stack parameter unk4 into the chain
  of non-overlapping values sharing one callee-saved register (as in the original) but gives that chain r4 and `this`
  r5, the original's registers renamed. Only these two allocation states turned up in ~6,000 variants.
- **Which variables exist, and where, moves the schedule; expression forms rarely do.** func_020875ac's case 2
  loaded the count between list[1] and list[0] only with one function-scope `u8 i` shared by all cases and the count
  read in the loop condition (`while (i < table[index].mCount)`); a count local before the loop is never scheduled
  ahead of list[0]. Macro vs inline, ternary vs if/else and index spellings all compiled the same.
- **A function-local static table schedules like const data.** mwcc knows no store can reach it, so its load moves
  above preceding stores, while the table stays in `.data` (a file-scope `const` would move it to `.rodata`).
  SpriteScreen::SetObjVramMode matched this way. The statics get `name$N` symbols, to check when the unit's data is
  claimed.
- **A u16/s16 stack parameter is loaded at the top of the entry block.** With a 32-bit type the same load sinks
  to its first consumer (or its home-slot copy joins the late r0 chain). It only helps when every consumer has the
  same narrow type: a width mismatch between caller and callee adds a conversion temporary and 8 bytes of frame.
  TextManager::PrintNumber matched with u16 width/height/unk12 on it and on Print (BattleCommandPage needed a u16
  `height` local to stay identical), MyroomCharaScene::PrintValue with `s16 suffix`.
- **Implicit narrowing at a short parameter is not shared between calls.** With `CellAnimation::Draw(u8, s16 x,
  s16 y, u8)` the original keeps one `x + 4` (int) and narrows it again at each call, and hoists the narrowed stack
  arguments in source order; an explicit `(s16)` cast passed to an `s32` parameter is shared and hoisted
  differently. Getting the callee's parameter types right matched FieldSelectScene::Draw and let
  several `(s16)`/`(s32)` casts at other call sites go.
- **A narrow parameter type changes the caller's spill scheduling.** A word-sized access to a u8/u16/s16 parameter's
  home slot shares the worst-case alias set with pointer accesses, so such stores and loads chain in the
  post-allocation scheduler (ties go to input order). BattleData::DrawBakuganAndNumber matched once its `value` was
  u16 and `flag` u8, which needed CellAnimation::Draw and DrawFrame's last parameter to be u8 as well; callers
  then pass u8 variables (a u32 variable gains an `and #0xff`).
- **`delete p;` versus `if (p) delete p;`.** mwcc emits the null check for a plain `delete` itself; an explicit `if`
  changes the block shape and the scheduling around it (BattleResult's destructors). Try both: BattleConfrontCard's
  func_0223f398 needed the explicit `if`.
- **`stm` where the original has two `str`s for stack arguments.** Declare the flag without an initialiser and assign
  it only in the arm that needs it (`BOOL has; if (...) {...} else { has = 0; ...}`), passing literals in the other
  arm (BattleCommandList::UpdatePage).
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
  MessageWindow::Update gets the same effect from an enum-typed copy of mScreen plus a named
  `SpriteManager &` (each alone is not enough). A value whose only
  consumer is one later call is substituted into that call; a compound assignment or a later read keeps it in place.
- **`(u8)` casts at a call site often stand in for the callee's parameter types.** Making `func_020231f4` take
  `u8, u8` matched three callers in ListScene.cpp. An `extern` object's declared size can matter too:
  stores at 0x1c/0x1d into `UnkStruct_0220f540` scheduled like the original only once the type was at least 0x3c bytes.
- **Code that leaves no instructions can still move the scheduler.** The choice between equally ready instructions
  depends on where they sit in the block; dummy calls before a block flip such ties (func_020843ac, func_02085220).
- **A negative constant stored with `strh` means a signed 16-bit destination.** `mov r0, #0xe000; sub r1, r0, #0x12000`
  (0xffffc000) stored to a halfword can't come from a `u16` field: every form folds to 0xc000. Only a signed 16-bit
  lvalue gives it (`*(s16 *) &p->mRotY = -0x4000;` as a placeholder, BattleCardCamera::Init).
- **A wrong constant can look like a scheduling difference.** When the original builds a value from a neighbouring
  constant (`mov r7, #34; sub r7, r7, #47`), that shows the true value (-13 where the source had 3,
  ScriptPlayer::func_0206fa20). Likewise a literal-pool word that differs is often a wrong divisor or constant
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
- **Which parameters are copied to the stack first.** mwcc emits every stack-parameter copy at entry, in parameter
  order (`-proc generic` shows it); the arm946e scheduler then moves a copy early only when its loaded value has a
  consumer in the same scheduled region, and leaves the rest late. A local with two definitions (`u32 spacing = unk12;`
  plus a second assignment) is not propagated, shares the parameter's home slot and pulls its copy early
  (TextManager::Print; its remaining early copies came from parameter widths: `u16 unk13` puts that parameter's
  copy at the top of the function).
- **A dead store can be load-bearing.** In BattleCpu::func_ov007_02231e2c, `spin = 30;` before a switch whose cases
  all overwrite `spin` keeps 30 in spin's register for later `MATH_Rand32(..., 30)` calls; `spin = 0` breaks it.
- **A `mov` where a spill reload was expected.** When a spilled value's register is still intact at its next use in
  the same block, mwcc replaces the reload with a `mov` (or drops it). So `mov rX, rY` next to a spill store, where
  the original has `ldr rX, [sp, #n]` (or the reverse), means the defining register was or wasn't reused before that
  use. How the variable is built moves this: `s16 x = base; x += off;` versus `s16 x = off + base;`, and statement
  order (BattleStatusPanels::DrawPanels).
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
  from an enum local: `UnkStruct_02065c40_Kind j = UNK_02065C40_KIND_0;` matched UpdateWait where `s16`/`int`
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
  `moveq`/`movne` order where `x = c ? 0 : 1` gives the reverse (`LoadFileAsync`).
- **Narrow read-modify-write toggles.** `f++; f &= 1;` as two statements on a `u8` field matches where a temporary
  (`u8 t = f + 1; f = t & 1;`) keeps the add in its own register (`ScriptPlayer::Update`).
- **Arguments a callee ignores.** If callers load a register before a call (e.g. `this` into r0) that the callee
  never reads, the callee still takes that parameter: `func_02084e64(ListScene *self)` is passed `self`
  although its body ignores it, and declaring it without the parameter makes the caller 8 bytes short.
- **Declare return values.** A function declared `void` that really returns a value changes the caller's
  register allocation: `LoadAndQueueUpload` returns `offset + size`, and declaring it `u32` (callers ignore the result)
  fixed runs of loads that kept a zero in a callee-saved register.
- **Runtime helpers.** `bl` to `0x0201f970` is `_s32_div_f`, i.e. a `/` in source: the dividend is in r0 and the
  divisor in r1, which often already holds a value the code uses elsewhere (keep it in a local).
- **`RequestScene` copies the context pointer to a local.** `SceneBase::RequestScene` is
  `SceneContext *context = mContext; context->mList->...->mNextScene = scene;`. Without the local, the
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
  would predicate (`BattleCommandList::UpdateListItems`).
- **A field set from an argument and then tested:** store first and test the field (`f = g; if (f != 0)`), not
  `if (g & 0xff)` (`CreateCommandListWindow`, `CreateCommandPageWindow`).
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
  So save-data style globals need their real size: `gSaveData` is `SaveData[1]`, padded to 0xff4.
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
  preheader (BattleCommandPage::DrawCard). Function-scope stack locals get slots in descending declaration order.
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
  the lower register. The cast also stops reuse of an already loaded base: `*(u8 *)&gSaveData[0].mUnk_24 |= 1`
  gets its own pool word. Don't change the shared SetFlagBit/TestFlagBit helpers: existing matches depend on them.
- **Narrowing changes scheduling, not just instructions.** `-(u8) (mFade / 4)` (or a `u8 level` local) and
  `-((u32) mFade >> 2)` emit the same `and`/`lsr`/`rsb`, but the `u8` form schedules the `rsb` before the next
  literal-pool load (BattleMoneyReward::Update).
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
