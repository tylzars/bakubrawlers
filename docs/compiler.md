# Compiler identification

How the toolchain settings in `tools/configure.py` were established. Each claim lists
the evidence and how to reproduce it.

## Result

| Code | Compiler | Flags |
|---|---|---|
| Game code (main + all overlays) | CodeWarrior for DS **2.0 sp2p2** (sp2p3/sp2p4 equivalent) | `-O4,p -ipa file -inline on,noauto -enum int -char signed -str noreuse -proc arm946e -fp soft -interworking -Cpp_exceptions off -RTTI off`, C++ |
| NitroSDK 4.2 (Nintendo prebuilt) | CodeWarrior for DS **2.0 sp1, sp1p2 or sp1p5** | `-O4,p -ipa file -enum int`, C |
| NitroSystem (NNS foundation, G3d, Gfd) | CodeWarrior **2.0** (every 2.0 build gives the same code; built with sp1p5 here) | `-O4,p -enum int` (`-ipa file` makes no difference), C |
| MSL C runtime (C parts) | CodeWarrior **2.0 sp1p5 or later** (`WriteLongLongDigits` is 0x24 bytes short on base..sp1p2) | `-O4,p` (`-ipa file -enum int` identical), C; soft float and division are hand-written assembly |
| CRI ADX audio | CodeWarrior **2.0** (every 2.0 build gives the same code) | `-O4,p -enum int` (`-ipa file` makes no difference), C |
| Mobiclip wrapper | CodeWarrior 2.0 (C++, the game's flags) | the decoder kernels at 0x02090b24+ are not identified |

Every game-code setting above is backed by an exact match that fails when that setting changes. For NitroSDK, `-char` and `-O` variants are unverified (see Open).

The NitroSDK functions written in C live in `libs/nds/src/` and build with `PATH_COMPILERS`/`PATH_DROP_FLAGS` in
`tools/configure.py`: 2.0/sp1p5 and the default inlining (not the game's `-inline on,noauto`). The matching
FX/GX code confirms `-O4,p -ipa file -enum int` with `-char signed`/`-str noreuse` harmless. Game code calls SDK
functions by their SDK names, declared `extern "C"` (`tools/rename_sdk.py` renames placeholder names).

## Tools

- `tools/probe_compilers.py <probe>`: compiles a hand-decompiled function with every
  installed compiler build and compares it byte-for-byte with the ROM, masking only
  each function's own relocation sites.
- `tools/sp_differential.py`: compiles an external, already-matching codebase with every
  2.0 build and lists functions whose post-link bytes differ between builds.
- `tools/match_external.py`: finds exact matches between those external objects and ROM functions.
- `tools/longmul_scan.py`: counts the pre-sp2p2 64-bit multiply idiom in disassembly.

## Compiler family: CodeWarrior for DS 2.0

The compiler was identified with 20 distinct real functions from main and ov000, ov001, ov006, ov007 and ov008,
decompiled as stand-alone probes
(constructors, destructors, virtual calls, singletons, soft float, 64-bit fixed point, switch
tables, register-heavy loops, SDK struct access, string tables). With the game flags
(1.2 builds at plain `-O4,p`, since they reject `-ipa`):

| Builds | Best exact matches |
|---|---|
| 1.2 (6 builds) | 2/20; sizes differ |
| 2.0 base … sp2 (7 builds) | 17/20 |
| **2.0 sp2p2, sp2p3, sp2p4** | **19/20, every one of the three** |
| DSi 1.x (9 builds) | 8/20 |

The remaining miss (`LoadAndQueueUpload`, 4 words) fails identically on all 25 builds, so it is a
source detail. To repeat the test on a function, decompile it into a stand-alone file and run
`python3 tools/probe_compilers.py <file> --flags='-O4,p -ipa file -inline on,noauto'`.

`func_0203eb4c` lies in a block of main (about 0x02030000–0x02054000)
that never uses the game's singleton guards and appears to be library code (CRI from about
0x0203E000). Its match shows that library was also built with CodeWarrior 2.0, but it isn't
counted as game-code evidence.

## Which 2.0 service pack

Synthetic test code showed no differences between 2.0 builds, so real code was used:
pret/pokeheartgold (4,771 functions) and zeldaret/ph (3,666 functions), compiled with
each 2.0 build. 68 and 19 functions differ after linking, splitting the builds into
`base | sp1–sp1p5 | sp1p6–sp2 | sp2p2 | sp2p3–sp2p4`.

**Game code is sp2p2 or later.** Through sp2, a 64-bit multiply by a value whose high word
is known to be zero emits `umull` plus two `mla` against a zeroed register. sp2p2 and
later drop them.
- Two whole functions reproduce exactly only on sp2p2/sp2p3/sp2p4:
  `RandomizePushOffsets` (random slot init via `MATH_Rand32`; 2.0 base…sp2 are 0x1C bytes
  larger, and 1.2 and DSi also miss) and `Push` (`FX_Mul` gauge update;
  2.0 base…sp2 are 8 bytes larger).
- `MATH_Rand32(ctx, 3)` in ov004 (`func_ov004_02219580`) is the short form. Compiling the
  NitroSDK inline, or any hand-written equivalent (shift, cast or inline-helper forms),
  gives the long form on every build up to sp2.
- `tools/longmul_scan.py` validates on known code (ph + SDK at sp2: 9 long forms; at
  sp2p2: 1 single-`mla` residual). Across Bakugan's game code (~1,000 `umull`/`smull`
  instructions, 73 of them 64-bit multiplies by an immediate), there is no sp2-style
  double-zero form and one sp2p2-style residual (`RandomizePushOffsets`, same shape as
  ph's sp2p2 residual).

**sp2p2, sp2p3 and sp2p4 are equivalent here.** Their only observed difference (4 of
pokeheartgold's C functions) is pooling the address of a file's own globals, which in
C++ gives the same duplicate pool entry on every build. Compiled with the game's exact
flags (including `-ipa file`), ph's 3,663 C++ functions are identical across all three.
sp2p2 is the lower bound; any of the three is correct for matching.

**NitroSDK is sp1–sp1p5.** 99 of pokeheartgold's NitroSDK functions exactly match this ROM:
- `DC_FlushAll`, `DC_FlushRange`, `DC_StoreAll`, `OSi_WaitVCount0`, `OSi_DoBoot` match sp1+ only.
- `OSi_ReloadRomData` matches base..sp1p5 only (register allocation changed in sp1p6).
- `OS_Sleep` has the pre-sp2p2 long multiply.

## Flags

| Flag | Evidence |
|---|---|
| `-O4,p` | `-O3,p` misallocates registers in `func_ov000_0221f474` and `UpdateSlots`; `-O4,s`/`-O2` keep the singleton accessor out of line and change `func_020253a0` and `RandomizePushOffsets` size |
| `-ipa file` | Inline-function local statics (e.g. guard `data_020c17cc`) are referenced from main and every overlay, so they are shared standalone sections. Without `-ipa file`, mwcc puts them in each file's `.bss` (checked on the frame-update probe). The SDK also loses 23/99 matches without it. |
| `-inline on,noauto` | `auto` inlines same-file calls (`CreateObj`, `Draw`) that ov006 keeps as `bl` |
| `-enum int` | Game: `LoadFileAsync` (file loader) inlines NitroSDK `FS_GetLength` on an `FSFile`, whose `prop.file.top/bottom` sit after two enum members; under `-enum min` the 4 field loads miss. SDK: `-enum min` loses 9/99 matches |
| `-char signed` | Game: `LoadEffectGroup` (effect loader) walks a plain `char` name table (`while (*p++)`, passed to `%s`) with `ldrsb`; under `-char unsigned` exactly that load differs |
| `-str noreuse` | ov006's `data_ov006_0221a544` initializer points at three identical copies of one path literal; `reuse`/`pool`/default merge them |
| `-Cpp_exceptions off` | `on` changes `new`-with-constructor codegen in ov006 |
| `-RTTI off` | vtables have a zero typeinfo slot (`data_ov006_0221a6a8 - 4`) |
| `-interworking` | Overlays use `bx lr` (216) and never `mov pc, lr`; without the flag leaf functions return with `mov pc, lr` |
| `-proc arm946e` | `arm9tdmi` has no `smulbb` and changes sizes |
| not set | `-constpool`, `-inline deferred` (reverses function order), `-rostr` (literals live in `.data`) |

## Open

- `ninja objdiff` passes one decomp.me preset (`mwcc_30_137`) for every unit; scratches
  for NitroSDK units must be switched to 2.0/sp1p5 (`mwcc_30_131`) by hand.
- NitroSDK `-char` and `-O` variants beyond `-O4,p` haven't been separated, because no matched
  SDK function depends on them.
- Main is not yet mapped into game and library regions (NitroSDK low, a library block around
  0x02030000–0x02054000, CRI/Mobiclip). Only functions with game idioms count as game evidence.
