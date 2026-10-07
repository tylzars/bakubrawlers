# Decompiling notes

Lessons from getting functions to match exactly. Compiler and flags: see `compiler.md`.

## Workflow

1. Pick a function or file. `ninja` then `tools/bin/dsd dis -c config/usa/arm9/config.yaml -a build/usa/asm --ual`
   gives disassembly to read from.
2. For a single function, iterate in a scratch file with a `// PROBE: <mangled symbol> <module> <address>` line:
   `python3 tools/probe_compilers.py <file> --only 2.0/sp2p2 --flags='-O4,p -ipa file -inline on,noauto' --show`
   prints a side-by-side diff with relocated words masked.
3. Put the code in its translation unit under `src/`, with an entry in `config/usa/arm9/**/delinks.txt` (see
   `CONTRIBUTING.md`). Shared classes go in `include/`, SDK declarations in `libs/nds/include/`.
4. Compile it (`ninja build/usa/src/<path>.o`), then rename the original symbols to the names your code uses:
   `python3 tools/sync_symbols.py build/usa/src/<path>.o <module> --write`. The tool locates each matching function
   by its bytes and reads every referenced address from the original code, so `symbols.txt` follows the source.
   A non-matching function of the same size can be placed with `--assume <mangled>=<addr>`.
5. Statics of inline functions (`_ZGVZ…`, `_ZZ…`, e.g. singleton instances and guards) are defined by every file
   that uses them. Mark them `weak` in `symbols.txt`. Compiled objects get their copies demoted to weak as well
   (`tools/demote_multidef.py`, run by the build), so main's original copy wins when a file is linked.
6. `ninja objdiff report` shows per-function and per-section results. When everything matches, add `complete`
   to the delinks entry and run `ninja`; the ROM must still match.

A function that doesn't match yet is kept in source with a `// non-matching` comment describing the difference.

## Known open problems

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
- **Guard load before a preceding store.** Before a singleton `Get()`, the original loads the guard word ahead of
  a store from the previous statement: the `TextManager::mUnk_5b` store (`StoreScene::PrintStatus`,
  `MapScene::LoadPanelText2`, ov000 `RenderName`, ov002 `0221dbd4`/`0221df48`, ov001 `0221c1d8`) or an unrelated
  global (`MapScene::func_ov005_0221bde8`, a `u16` copy inside `data_020d48f0`). No 2.0 or DSi compiler build
  moves a load above a store on its own, for compiler guards or plain globals; the only case found that does is a
  load of a `const`-qualified global. Changing the field type (`bool`, `s8`, bitfields), writing through a
  reference, comma expressions, inline setters and `const` singletons all leave the store first.
- **Zero kept in a callee-saved register across `func_02025b64` calls.** In some runs of resource loads the
  original materialises one zero (`mov r4, #0`) and reuses it for the stack arguments and the offset of the next
  two or three calls (ov001 `0221b24c`, `0221f4e8`; ov002 `0221fb24`; `MapScene::LoadMapBackground`). Plain
  literals, a zero local, an inline helper with a zero parameter, a one-trip loop counter and `n * 4` with `n = 0`
  all fold to fresh `mov rX, #0` per call. Not solved yet.

## Source patterns that matter

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
