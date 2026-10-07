#!/usr/bin/env python3
"""Lists functions in decompiled units that do not match yet (from build/<ver>/report.json; run `ninja report` first)."""
import json
import sys
from pathlib import Path

report = json.loads(Path(sys.argv[1] if len(sys.argv) > 1 else "build/usa/report.json").read_text())
for unit in report["units"]:
    if not unit["name"].startswith("src/"):
        continue
    for fn in unit.get("functions", []):
        pct = fn.get("fuzzy_match_percent", 0)
        if pct < 100:
            name = fn.get("metadata", {}).get("demangled_name") or fn["name"]
            print(f"{pct:6.2f}%  {unit['name'][4:]:34} {name}")
