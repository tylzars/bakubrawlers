# Bakugan: Battle Brawlers (DS) decomp

Matching decompilation of *Bakugan: Battle Brawlers* (US, `B6RE`), built with [ds-decomp](https://github.com/AetiasHax/ds-decomp) (`dsd`), Metrowerks mwccarm/mwldarm under [wibo](https://github.com/decompals/wibo), and [objdiff](https://github.com/encounter/objdiff).

Target: `orig/baserom_us.nds`, sha1 `dab7df53468533382d39631b488c491c7d1b78ca`.

## Setup
1. Put the ROM at `orig/baserom_us.nds` (never committed).
2. `tools/build_dsd.sh`: builds `dsd` v0.12.1 with a ds-rom banner-size fix (`tools/patches/`). Stock releases can't rebuild this ROM.
3. `python3 tools/configure.py us && ninja`: downloads wibo/mwccarm/objdiff, extracts, delinks, links, rebuilds the ROM, and checks the sha1.
4. `git config core.hooksPath .githooks`: enables the pre-push check that the progress graph is current.

Optional: `orig/biosnds7.bin` (ARM7 BIOS). Without it, `tools/fix_header.py` restores the secure-area CRC. The secure area contains no code.

## Workflow
- `ninja objdiff`: writes `objdiff.json` for the objdiff GUI. `ninja report`: progress report.
- Before pushing: `ninja report && tools/progress.py update`, then commit `README.md` and `progress/`. The pre-push hook refuses stale graphs.
- Decompile a TU: add an entry to `config/us/arm9/**/delinks.txt`, write `src/...c`, and mark it `complete` when it matches. Rerun `configure.py` when you add source files.

## Notes
- NitroSDK 4.2 (relstep 30009); CRI ADX/Sofdec (built Apr 2008) and Actimagine Mobiclip 1.1.1 are linked in.
- Overlay entries are kept via `FORCE_ACTIVE` (`tools/lcf_force_active.py`), because main reaches every overlay through the shared address 0x02219440.

## Progress
<!-- progress:start -->
![Decompilation progress over time](progress/progress.svg)

| Measure | Progress |
|---|---|
| Matched code | 0.00% (0 / 1,240,936 bytes) |
| Linked code (complete units) | 0.00% (0 bytes) |
| Overall accuracy (fuzzy) | 0.00% |
| Matched functions | 0 / 4,505 |

_Last updated 2026-10-07. Full history: [`progress/history.csv`](progress/history.csv)._
<!-- progress:end -->
