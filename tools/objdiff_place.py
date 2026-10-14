#!/usr/bin/env python3
"""Points the report at the right object for functions placed apart by lcf_edits.txt.

A `place <stub>.o <function> <unit>.o` edit leaves the stub unit's compiled object empty; its function is compiled in
<unit>. This makes objdiff compare the stub's original against <unit>'s object, so the function is scored where it is
really defined.

  objdiff_place.py objdiff.json config/usa/arm9/lcf_edits.txt
"""
import json
import sys
from pathlib import Path

cfg_path, edits_path = Path(sys.argv[1]), Path(sys.argv[2])
cfg = json.loads(cfg_path.read_text())
stem = lambda o: o.removesuffix(".o")
for line in edits_path.read_text().splitlines():
    words = line.split("#", 1)[0].split()
    if not words or words[0] != "place":
        continue
    stub, _, unit = words[1:]
    units = {Path(u["name"]).name: u for u in cfg["units"]}
    if stem(stub) in units and stem(unit) in units:
        units[stem(stub)]["base_path"] = units[stem(unit)]["base_path"]
cfg_path.write_text(json.dumps(cfg, indent=2) + "\n")
