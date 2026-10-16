# Contributing

## Layout
| Path | Contents |
| --- | --- |
| `config/usa/` | `dsd` configuration: symbols, relocations and translation-unit boundaries (`delinks.txt`) |
| `src/Main/`, `src/ITCM/` | Game code from the main ARM9 binary and ITCM |
| `src/NN_Name/` | Game code from overlay `NN`, named once its purpose is known (e.g. `06_GameOver`); unnamed overlays use just the number |
| `include/` | Game headers, grouped by subsystem (`include/Scene/`, ...) |
| `libs/nds/` | C code and headers for the NitroSDK functions the game links (built with an older compiler, see `docs/compiler.md`) |
| `libs/c/`, `libs/cpp/` | C and C++ runtime pieces |
| `tools/` | Build scripts and analysis tools |
| `docs/` | Notes on the compiler, decompiling and the game |
| `extract/`, `build/` | Your ROM, its extraction and build output (never committed) |

## Naming
- Source files are named after the class they define: `src/06_GameOver/Scene/GameOverScene.cpp` with its header at
  `include/Scene/GameOverScene.hpp`. C++ headers use `.hpp`, C headers `.h`.
- Classes and methods use `PascalCase`; members use an `m` prefix (`mState`).
- Anything not yet understood keeps an address-based name: functions `func_<addr>` / `func_ovNNN_<addr>`, data
  `data_<addr>`, members `mUnk_<offset>`, virtual slots `vfunc_<offset>`.
- Annotate struct members and vtables with their offsets: `/* 2c */ u8 mState;`.

## Adding a translation unit
1. Add the file and its section ranges to the right `delinks.txt` (`dsd` splits the original object at those
   boundaries). Boundaries that are guesses should carry a `// provisional` comment above the entry.
2. Write the code, then use `ninja objdiff` and the objdiff GUI, or `tools/probe_compilers.py` for single functions,
   until every function and data section matches.
3. Mark the entry `complete` and run `ninja`. The ROM must still match, so the compiled object is linked in place of
   the original.
4. Before pushing, run `ninja report && tools/progress.py update` and commit the updated graph.

## Rules
- Never commit anything taken from the game: ROMs, assets, extracted files or disassembly. Text the game code
  itself uses (string literals, file paths) belongs in source like any other constant.
- Don't copy code or prose from other projects. Write SDK headers yourself from the public API and observed codegen.
- Data the code uses is written as typed source (structs and arrays with their values), not as raw byte dumps.
  Bytes with no meaning as data (the secure-area filler) are read from your ROM at build time (`tools/expand_incbin.py`).
- Third-party code: the sixteen `libs/msl/src/` math files `e_*.c`, `k_*.c` and `s_*.c` are fdlibm, which the game's
  runtime library uses; they keep fdlibm's copyright and permission notice.
- Format C/C++ with the repository's `.clang-format`.
