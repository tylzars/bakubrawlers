#!/usr/bin/env python3
"""Prepends a FORCE_ACTIVE block to the dsd-generated LCF.

Main reaches every overlay through a call to the shared load address
(0x02219440), which dsd records as an ambiguous relocation and links against
only the first overlay. With -dead, overlays whose entry nothing else
references get stripped to nothing. Keeping each overlay's entry function
active restores the original reachability.

The entry symbol is looked up by address in each overlay's symbols.txt, so
renaming it there is picked up automatically.

It also keeps the data symbols listed in <config-dir>/force_active.txt (one per line, `#` comments): objects the
original link kept although nothing references them, which -dead would otherwise strip from a unit linked from
source.

It also applies <config-dir>/lcf_edits.txt (one edit per line, `#` comments), for what dsd's per-unit ranges can't say:
  place <stub>.o <function> <unit>.o   the stub unit's line becomes OBJECT(<function>, <unit>.o): one function of a
                                       unit placed apart from the rest of it
  alias <symbol> <base> <offset>       defines <symbol> = <base> + <offset>, for names other units use for a point
                                       inside an object of a linked unit (a field of a singleton instance)

(mwldarm's -force_active flag would be simpler but segfaults under wibo.)
"""
import argparse
import re
from pathlib import Path

parser = argparse.ArgumentParser()
parser.add_argument("lcf_in", type=Path)
parser.add_argument("lcf_out", type=Path)
parser.add_argument("--config-dir", type=Path, required=True, help="config/<ver>/arm9")
args = parser.parse_args()

TEXT_START = re.compile(r"^\s*\.text\s+start:(0x[0-9a-fA-F]+)")
SYMBOL = re.compile(r"^(\S+)\s+kind:function\S*\s+addr:(0x[0-9a-fA-F]+)")

entries: list[str] = []
for ov_dir in sorted((args.config_dir / "overlays").iterdir()):
    base = None
    for line in (ov_dir / "delinks.txt").read_text().splitlines():
        if m := TEXT_START.match(line):
            base = int(m.group(1), 16)
            break
    if base is None:
        raise SystemExit(f"{ov_dir}: no .text section in delinks.txt")
    for line in (ov_dir / "symbols.txt").read_text().splitlines():
        if (m := SYMBOL.match(line)) and int(m.group(2), 16) == base:
            entries.append(m.group(1))
            break
    else:
        raise SystemExit(f"{ov_dir}: no function symbol at overlay base {base:#010x}")

extra = args.config_dir / "force_active.txt"
if extra.exists():
    for line in extra.read_text().splitlines():
        name = line.split("#", 1)[0].strip()
        if name:
            entries.append(name)

lcf = args.lcf_in.read_text()
# Linker-defined constants the linker script has to provide (values read from the original's literal pools):
# overlay ids under their SDK names, and the IRQ/system stack sizes.
sdk = [f"    SDK_OVERLAY_OV{n:03d}_ID = {n};" for n in range(lcf.count("_ID = "))]
sdk += ["    SDK_IRQ_STACKSIZE = 0x1000;", "    SDK_SYS_STACKSIZE = 0;"]
edits = args.config_dir / "lcf_edits.txt"
if edits.exists():
    for line in edits.read_text().splitlines():
        words = line.split("#", 1)[0].split()
        if not words:
            continue
        if words[0] == "place":
            stub, func, unit = words[1:]
            old = f"        {stub}(.text)\n"
            if old not in lcf:
                raise SystemExit(f"lcf_edits: no line for {stub} in the LCF")
            lcf = lcf.replace(old, f"        OBJECT({func}, {unit})\n", 1)
        elif words[0] == "alias":
            name, base, offset = words[1:]
            sdk.append(f"    {name} = {base} + {offset};")
        else:
            raise SystemExit(f"lcf_edits: unknown edit {words[0]!r}")
lcf = lcf.replace("SECTIONS {\n", "SECTIONS {\n" + "\n".join(sdk) + "\n", 1)
args.lcf_out.write_text(f"FORCE_ACTIVE {{ {', '.join(entries)} }}\n\n{lcf}")
