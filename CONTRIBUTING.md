# Contributing

## Layout
| Path | Contents |
| --- | --- |
| `config/usa/` | `dsd` configuration: symbols, relocations and translation-unit boundaries (`delinks.txt`) |
| `src/Main/` | Game code from the main ARM9 binary, grouped by subsystem (`Battle/`, `Game/`, `Graphics/`, `Scene/`, `System/`, `Text/`, `UI/`, `Video/`; `Unknown/` for units whose role isn't settled) |
| `src/NN_Name/` | Game code from overlay `NN`, named once its purpose is known (e.g. `06_GameOver`); unnamed overlays use just the number |
| `include/` | Game headers, grouped by subsystem (`include/Scene/`, ...) |
| `libs/nds/` | C code and headers for the NitroSDK functions the game links (built with an older compiler, see `docs/compiler.md`) |
| `libs/nns/` | NitroSystem code the game links (G3d, sound, heaps, VRAM managers) |
| `libs/cri/` | CRI middleware the game links (ADX / AHX audio, streams, file devices) |
| `libs/msl/`, `libs/c/`, `libs/cpp/` | C and C++ runtime pieces |
| `tools/` | Build scripts and analysis tools |
| `docs/` | Notes on the compiler, decompiling and the game |
| `extract/`, `build/` | Your ROM, its extraction and build output (never committed) |

## Naming
- Source files are named after the class they define: `src/06_GameOver/Scene/GameOverScene.cpp` with its header at
  `include/Scene/GameOverScene.hpp`. C++ headers use `.hpp`, C headers `.h`.
- Classes, structs, enum types, methods and free functions use `PascalCase`; members use an `m` prefix (`mState`).
- File-scope statics use an `s` prefix (`sFrameCount`), globals shared between units a `g` prefix (`gGameData`).
- Locals and parameters use `camelCase`; `i`/`j`/`k` are fine for loop counters.
- Enum values are `UPPER_SNAKE` prefixed with their type (`BATTLE_MODE_TAG`); `#define` constants are `UPPER_SNAKE`.
- Magic numbers get a named constant once their meaning is clear from the code, but a variable's or member's type
  never changes to an enum type: the compiler treats enum-typed values differently and the output would move. New
  constants are `#define`s; a `#define` keeps the literal's value and type exactly (suffix included, `0x8000u`).
- A constant names a value of a set: the values one member, parameter or return value takes. All the set's names
  share a prefix (`SCENE_`, `UPLOAD_KIND_`, `WINDOW_ITEM_`) and go in the header that declares the API carrying the
  value, or at the top of the one `.cpp` that uses them. Every site of the set uses the names; a value the code gives
  no meaning (a sentinel, a count, a value doing another job) stays a literal with a comment. Coordinates, sizes and
  one-off waits stay literals.
- Recurring words keep one meaning everywhere: `m...Timer` counts down, `m...Frames` counts up, `m...FrameCount` is
  a fixed length, `m...Handle` holds index + 1 (0 = none), `m...Visible` / `m...Enabled` are 1 when drawn / active,
  `Update<X>` runs every frame, `Load<X>` reads a file, `Init<X>` runs once, `Setup<X>` can run again. In `libs/cri`
  function names put the type first (`AdxStart`, `CriStmAlloc`); a locked wrapper takes the plain name and the
  function it wraps `...Inner` or `...NoLock`.
- A name has to be supported by what this repository shows: what the code does, the game's own strings and file
  paths, and names already given. New names carry no address (symbols.txt keeps it).
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
