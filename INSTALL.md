# Building

## Requirements
- Linux (the build runs the Windows compiler through wibo)
- Python 3.10+ with `pyelftools`
- Ninja, Git, and a Rust toolchain (`cargo`) for building `dsd`

## Steps
1. Put your ROM at `extract/baserom_bakugan_usa.nds` and check its SHA-1 against [extract/README.md](extract/README.md).
2. Build the patched `dsd` (stock releases size version-3 banners incorrectly):
   ```shell
   tools/build_dsd.sh
   ```
3. Generate the build script and build:
   ```shell
   python3 tools/configure.py usa
   ninja
   ```
   The first run downloads wibo, the CodeWarrior compilers and objdiff into `tools/`, extracts the ROM, splits it
   into objects, links it and checks the result against `bakugan_usa.sha1`.
4. Enable the repository hooks (the pre-push hook keeps the progress graph current):
   ```shell
   git config core.hooksPath .githooks
   ```

## Matching the header
Without an ARM7 BIOS dump, the secure-area CRC in the ROM header is restored by `tools/fix_header.py`. With one
(`arm7_bios.bin` in the repository root), `dsd` writes the header checksum at 0x6C itself. Either way the ROM matches.

## Useful targets
| Command | Result |
| --- | --- |
| `ninja` | Build the ROM and verify its SHA-1 |
| `ninja objdiff` | Write `objdiff.json` for the objdiff GUI |
| `ninja report` | Write `build/usa/report.json` (matching progress) |
| `ninja check` | SHA-1 plus `dsd check symbols` |
