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

- **Two literal-pool entries for one address.** `func_02023504` (FileSystem.cpp) loads 0x020d53e4 from two
  different pool words, so two relocations (different symbols, or symbol plus offset) resolve to the same address
  in the original. The source models it with a second extern, `data_020d53e4_h`; the real layout of the loader's
  statics (header word, registration node, two MI contexts, state fields) is still to be worked out.
- **Order of identical string literals.** With `-str noreuse`, every copy of a literal gets its own object
  (`@N`, in its own `.data` section). The compiler emits the copies of one string in a permuted order, and the
  linker keeps that section order. `GameOverScene.cpp`'s result-window table uses five copies of one path;
  table entries 0..4 point at copies 4 2 0 1 3 (in emission order) where the original has 2 4 3 1 0. This is the
  only thing stopping that unit from linking (`complete`). What is known:
  - The permutation is stable under renaming, under extra declarations or literal-using functions placed
    before the tables, under `-sym on/off` and under small edits inside other functions.
  - It changes when whole functions are removed (even ones without strings, e.g. the destructor), when the
    tables move into functions, and when other tables of identical strings are added or moved.
  - Named `char[]` arrays instead of literals are permuted the same way, so they are not a way around it.
  `build/lit/both.py`-style probing (compile, read the relocations of the table, rank the target sections) is the
  quickest way to test ideas.
  - New (see "Data layout within a section"): the literals of data initializers are sorted by size together with
    every other object of the file, `.bss` included. Putting `Text/TextManager.hpp` first changes the order
    of GameOverScene's five copies, and so does moving a table without strings. Literals used inside functions are
    not in that list (they come out in creation order). No combination of table order and the first two singleton
    includes reproduces the original yet; `build/lit2/gos.py` measures the order for a source variant.
- **Guard load before a preceding store (solved).** mwcc lets a following load (e.g. the next singleton's guard)
  move above a store only when the store's offset is below half of the declared size of the object stored into.
  `TextManager` was declared 0x5c bytes, so the `mUnk_5b` store (offset 0x5b) blocked it; with its real size,
  0xe8 (an unused trailing member, so `Widths()` keeps its code), 22 functions match. So when a store seems to
  pin a later load, check the declared size of the stored-to object.

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
predictions from `tools/data_order.py` are only a hint. `python3 tools/fix_layout.py <unit>` is the reliable
route: it tries putting the unit's singleton headers first (one to three of them, in every order), recompiles
and keeps the first order for which `data_ranges.py` reports no layout conflicts. `.clang-format` keeps include
order (`SortIncludes: Never`), and it is load-bearing.

Other findings from linking:
- **Two classes, one file.** The ConfrontEmblem0N classes were compiled in the same file as their
  ConfrontBg0N/Confront03 partner: the emblem's strings sit in the partner's data block, after both vtables.
  An object that lies in a neighbour's block in the original means the code using it belongs to that file.
- **Dead constructor copies.** mwcc emits separate C1 and C2 constructors (and D1/D2) for some classes. The
  original link dropped the unused copy and the strings only it used; `data_ranges.py` ignores sections used only
  by functions the original doesn't have.
- When no include order works, look for a boundary problem before anything else (IBattle: its first functions use
  a WirelessManager node that lies among the previous file's nodes).

## Source patterns that matter

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
