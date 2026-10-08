#!/usr/bin/env python3
"""Renames symbols.txt entries inside complete units to the names the linked binary gives those addresses.

A unit marked `complete` is linked from its compiled object, so its data symbols are the compiler's: named
statics, vtables, and `@N` for string literals and other anonymous objects. `dsd check symbols` looks every
symbols.txt entry up by name, so after marking a unit complete (and whenever its `@N` numbering shifts), run

  ninja && python3 tools/sync_linked_symbols.py [--write]

Only entries whose current name is missing from the linked binary are touched; file-level statics and `@N`
objects get `local`. Functions are left alone (sync_symbols.py names those from source).
"""
import argparse
import re
from pathlib import Path

from elftools.elf.elffile import ELFFile

from link_check import CFG, delinks_files, parse_units
from sync_symbols import retarget_relocs

ROOT = Path(__file__).resolve().parent.parent
LINE = re.compile(r"^(\S+) (kind:\S+) addr:(0x[0-9a-f]+)(.*)$")
SECTION_OF = {"main": "ARM9", "itcm": "ITCM", "dtcm": "DTCM"}


def symbols_path(module):
    if module == "main":
        return CFG / "symbols.txt"
    if module in ("itcm", "dtcm"):
        return CFG / module / "symbols.txt"
    return CFG / "overlays" / module / "symbols.txt"


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--write", action="store_true")
    ap.add_argument("--elf", default="build/usa/arm9.o")
    args = ap.parse_args()

    with open(ROOT / args.elf, "rb") as fh:
        elf = ELFFile(fh)
        secname = {i: s.name for i, s in enumerate(elf.iter_sections())}
        names = set()
        at: dict[tuple[str, int], list] = {}
        spans = []
        binding: dict[str, str] = {}
        for s in elf.get_section_by_name(".symtab").iter_symbols():
            if not isinstance(s["st_shndx"], int) or not s.name or s.name.startswith(("$", ".")):
                continue
            names.add(s.name)
            binding.setdefault(s.name, s["st_info"]["bind"])
            if s["st_info"]["type"] in ("STT_OBJECT", "STT_NOTYPE"):
                mod = secname[s["st_shndx"]]
                at.setdefault((mod, s["st_value"]), []).append((s.name, s["st_info"]["bind"]))
                if s["st_size"]:
                    spans.append((mod, s["st_value"], s["st_value"] + s["st_size"], s.name))

    total = 0
    for module, delinks in delinks_files():
        mod = SECTION_OF.get(module, module.upper())
        ranges = [r for u in parse_units(delinks) if u["complete"] for name, r in u["ranges"].items()
                  if name != ".text"]
        if not ranges:
            continue
        path = symbols_path(module)
        lines = path.read_text().splitlines()
        changed = 0
        all_ranges = [r for u in parse_units(delinks) if u["complete"] for r in u["ranges"].values()]
        for i, line in enumerate(lines):
            m = LINE.match(line)
            if not m:
                continue
            addr = int(m.group(3), 16)
            if m.group(1) in names:
                # Binding attributes follow the linked binary inside complete units.
                if any(lo <= addr < hi for lo, hi in all_ranges):
                    bind = binding[m.group(1)]
                    attrs = re.sub(r"\s*\b(local|weak)\b", "", m.group(4))
                    attrs += " local" if bind == "STB_LOCAL" else " weak" if bind not in ("STB_GLOBAL",) else ""
                    if attrs != m.group(4):
                        lines[i] = f"{m.group(1)} {m.group(2)} addr:{m.group(3)}{attrs}"
                        print(f"{module}: {m.group(1)}: '{m.group(4).strip()}' -> '{attrs.strip()}'")
                        changed += 1
                continue
            if m.group(2).startswith(("kind:function", "kind:label")):
                continue
            if not any(lo <= addr < hi for lo, hi in ranges):
                continue
            cands = at.get((mod, addr), [])
            if not cands:
                inside = next(((n, lo) for md, lo, hi, n in spans if md == mod and lo < addr < hi), None)
                if inside:  # an interior address (e.g. dsd's vtable symbol 8 bytes into the vtable)
                    start = inside[1]
                    has_start = any((mm := LINE.match(ln or "")) and int(mm.group(3), 16) == start for ln in lines)
                    if has_start:
                        print(f"{module}: {m.group(1)} at {addr:#x} is inside {inside[0]}: removed")
                        lines[i] = None
                    else:
                        print(f"{module}: {m.group(1)} at {addr:#x} is inside {inside[0]}: moved to its start")
                        lines[i] = f"{inside[0]} {m.group(2)} addr:{start:#010x}"
                    changed += 1
                    if args.write:
                        retarget_relocs(addr, start)
                else:
                    print(f"{module}: {m.group(1)} at {addr:#x}: nothing in the linked binary")
                continue
            new, bind = sorted(cands, key=lambda c: (c[1] != "STB_GLOBAL", c[0]))[0]
            rest = re.sub(r"\s*\blocal\b", "", m.group(4))
            lines[i] = f"{new} {m.group(2)} addr:{m.group(3)}{rest}" + (" local" if bind == "STB_LOCAL" else "")
            print(f"{module}: {m.group(1)} -> {new}" + (" (local)" if bind == "STB_LOCAL" else ""))
            changed += 1
        if changed and args.write:
            path.write_text("\n".join(ln for ln in lines if ln is not None) + "\n")
        total += changed
    print(f"{total} renamed" + ("" if args.write else " (dry run)"))


if __name__ == "__main__":
    main()
