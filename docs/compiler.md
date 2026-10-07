# Compiler identification

How the toolchain settings in `tools/configure.py` were established. Each claim lists
the evidence and how to reproduce it.

## Result

| Code | Compiler | Flags |
|---|---|---|
| Game code (main + all overlays) | CodeWarrior for DS **2.0 sp2p2** (sp2p3/sp2p4 equivalent) | `-O4,p -ipa file -inline on,noauto -enum int -char signed -str noreuse -proc arm946e -fp soft -interworking -Cpp_exceptions off -RTTI off`, C++ |
| NitroSDK 4.2 (Nintendo prebuilt) | CodeWarrior for DS **2.0 sp1, sp1p2 or sp1p5** | `-O4,p -ipa file -enum int`, C |
| CRI ADX/Sofdec, Mobiclip | vendor prebuilt, not identified | keep delinked |

`-char signed` (everywhere) and `-enum int` (game code) are not yet observed in this binary (see the end of this doc).

## Tools

- `tools/probe_compilers.py <probe>`: compiles a hand-decompiled function with every
  installed compiler build and compares it byte-for-byte with the ROM, masking only
  each function's own relocation sites.
- `tools/sp_differential.py`: compiles an external, already-matching codebase with every
  2.0 build and lists functions whose post-link bytes differ between builds.
- `tools/match_external.py`: finds exact matches between those external objects and ROM functions.
- `tools/longmul_scan.py`: counts the pre-sp2p2 64-bit multiply idiom in disassembly.

## Compiler family: CodeWarrior for DS 2.0

The compiler was identified with 11 distinct real functions, decompiled as stand-alone probes, from ov000, ov006 and main (constructors, destructors,
virtual calls, singletons, soft float, switch tables). Compiled at `-O4,p`:

| Builds | Exact matches |
|---|---|
| 1.2 (6 builds) | at most 2/11; sizes differ |
| **2.0 (10 builds)** | **9/11** |
| DSi 1.x (9 builds) | at most 5/11 |

The two misses fail identically on all 25 builds, so they are source details, not the toolchain.

## Which 2.0 service pack

Synthetic test code showed no differences between 2.0 builds, so real code was used:
pret/pokeheartgold (4,771 functions) and zeldaret/ph (3,666 functions), compiled with
each 2.0 build. 68 and 19 functions differ after linking, splitting the builds into
`base | sp1–sp1p5 | sp1p6–sp2 | sp2p2 | sp2p3–sp2p4`.

**Game code is sp2p2 or later.** Through sp2, a 64-bit multiply by a value whose high word
is known to be zero emits `umull` plus two `mla` against a zeroed register. sp2p2 and
later drop them.
- `MATH_Rand32(ctx, 3)` in ov004 (`func_ov004_02219580`) is the short form. Compiling the
  NitroSDK inline, or any hand-written equivalent (shift, cast or inline-helper forms),
  gives the long form on every build up to sp2.
- `tools/longmul_scan.py` validates on known code (ph + SDK at sp2: 9 long forms; at
  sp2p2: 1 single-`mla` residual). Across Bakugan's game code (~1,000 `umull`/`smull`
  instructions, 73 of them 64-bit multiplies by an immediate), there is no sp2-style
  double-zero form and one sp2p2-style residual (`func_ov007_02232c88`, same shape as
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
| `-O4,p` | `-O3,p` misallocates registers in `func_ov000_0221f474`; `-O4,s`/`-O2` keep the singleton accessor out of line and change `func_020253a0` size |
| `-ipa file` | Inline-function local statics (e.g. guard `data_020c17cc`) are referenced from main and every overlay, so they are shared standalone sections. Without `-ipa file`, mwcc puts them in each file's `.bss` (checked on the frame-update probe). The SDK also loses 23/99 matches without it. |
| `-inline on,noauto` | `auto` inlines same-file calls (`CreateObj`, `Draw`) that ov006 keeps as `bl` |
| `-enum int` | SDK only: `-enum min` loses 9/99 SDK matches. For game code it is inherited (same status as `-char signed`) |
| `-str noreuse` | ov006's `data_ov006_0221a544` initializer points at three identical copies of one path literal; `reuse`/`pool`/default merge them |
| `-Cpp_exceptions off` | `on` changes `new`-with-constructor codegen in ov006 |
| `-RTTI off` | vtables have a zero typeinfo slot (`data_ov006_0221a6a8 - 4`) |
| `-interworking` | Overlays use `bx lr` (216) and never `mov pc, lr`; without the flag leaf functions return with `mov pc, lr` |
| `-proc arm946e` | `arm9tdmi` has no `smulbb` and changes sizes |
| not set | `-constpool`, `-inline deferred` (reverses function order), `-rostr` (literals live in `.data`) |

## Open

- `-char signed` (game and SDK) and `-enum int` (game code): no matched function depends on
  them yet. Both are NitroSDK build defaults. Question these
  first on a char- or enum-related mismatch.
- `ninja objdiff` passes one decomp.me preset (`mwcc_30_137`) for every unit; scratches
  for NitroSDK units must be switched to 2.0/sp1p5 (`mwcc_30_131`) by hand.
- NitroSDK `-char` and `-O` variants beyond `-O4,p` haven't been separated, because nothing
  matched so far depends on them.
