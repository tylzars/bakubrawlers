#!/usr/bin/env python3
"""Prepends a FORCE_ACTIVE block to the dsd-generated LCF.

Main reaches every overlay through a call to the shared load address
(0x02219440), which dsd records as an ambiguous relocation and links against
only the first overlay. With -dead, overlays whose entry nothing else
references get stripped to nothing. Keeping each overlay's entry function
active restores the original reachability.

The entry symbol is looked up by address in each overlay's symbols.txt, so
renaming it there is picked up automatically.

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

lcf = args.lcf_in.read_text()
args.lcf_out.write_text(f"FORCE_ACTIVE {{ {', '.join(entries)} }}\n\n{lcf}")
