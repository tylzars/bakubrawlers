#!/usr/bin/env python3
"""Lists functions whose set of callees differs from the original's, including functions that don't match yet.

check_calls.py compares call targets instruction by instruction, which needs identical code. This compares, per
function, the multiset of `bl` targets (by address) in our compiled object with the `bl`/`blx` targets inside the
original function's address range, so wrong callees (e.g. a neighbouring cache routine, a signed instead of an
unsigned division helper) show up while the function still differs. Run after `ninja`.

Usage: check_callees.py [object.o ...]   (default: every object under build/usa/src and build/usa/libs)
"""
import struct
import sys
from collections import Counter
from pathlib import Path

from elftools.elf.elffile import ELFFile

sys.path.insert(0, str(Path(__file__).parent))
from check_calls import ROOT, load_symbols, module_bytes, module_of  # noqa: E402


def original_callees(blob, base, start, size):
    out = Counter()
    for site in range(start, start + size, 4):
        if not base <= site < base + len(blob) - 3:
            continue
        word = struct.unpack_from("<I", blob, site - base)[0]
        cond = word >> 28
        if word & 0x0F000000 == 0x0B000000 and cond != 0xF:  # BL
            d = word & 0xFFFFFF
            d -= (1 << 24) if d & 0x800000 else 0
            out[(site + 8 + d * 4) | 1] += 1
        elif cond == 0xF and word & 0x0E000000 == 0x0A000000:  # BLX imm
            d = word & 0xFFFFFF
            d -= (1 << 24) if d & 0x800000 else 0
            out[(site + 8 + d * 4 + (word >> 24 & 1) * 2) | 1] += 1
    return out


def load_sizes():
    import re
    sizes = {}
    files = [ROOT / "config/usa/arm9/symbols.txt", ROOT / "config/usa/arm9/itcm/symbols.txt",
             *sorted((ROOT / "config/usa/arm9/overlays").glob("*/symbols.txt"))]
    for path in files:
        for line in path.read_text().splitlines():
            m = re.match(r"(\S+) kind:function\(\w+,size=(0x[0-9a-f]+)\)", line)
            if m:
                sizes.setdefault(m.group(1), int(m.group(2), 16))
    return sizes


def main():
    tables = load_symbols()
    sizes = load_sizes()
    objs = [Path(p).resolve() for p in sys.argv[1:]] or sorted(
        [*(ROOT / "build/usa/src").rglob("*.o"), *(ROOT / "build/usa/libs").rglob("*.o")])
    names_at = {}
    for t in tables.values():
        for n, a in t.items():
            names_at.setdefault(a | 1, n)
    problems = 0
    for obj in objs:
        module = module_of(obj)
        if module is None:
            continue
        base, blob = module_bytes(module)
        local = tables[module]
        with open(obj, "rb") as fh:
            elf = ELFFile(fh)
            syms = list(elf.get_section_by_name(".symtab").iter_symbols())
            funcs = [s for s in syms if s["st_info"]["type"] == "STT_FUNC" and isinstance(s["st_shndx"], int)
                     and s["st_size"] and s.name in local]
            ours = {f.name: Counter() for f in funcs}
            unknown = {f.name: False for f in funcs}
            for sec in elf.iter_sections():
                if not sec.name.startswith(".rel"):
                    continue
                for r in sec.iter_relocations():
                    if r["r_info_type"] not in (1, 28, 29):
                        continue
                    fn = next((f for f in funcs if f["st_shndx"] == sec["sh_info"]
                               and f["st_value"] <= r["r_offset"] < f["st_value"] + f["st_size"]), None)
                    if fn is None:
                        continue
                    target = syms[r["r_info_sym"]].name
                    addr = local.get(target, tables["main"].get(target))
                    if addr is None:
                        addr = next((t[target] for t in tables.values() if target in t), None)
                    if addr is None:
                        unknown[fn.name] = True
                        continue
                    ours[fn.name][addr | 1] += 1
            for f in funcs:
                if unknown[f.name]:
                    continue
                # scan the original over the ORIGINAL function's size, not ours
                theirs = original_callees(blob, base, local[f.name], sizes.get(f.name, f["st_size"]))
                if ours[f.name] != theirs:
                    extra = ours[f.name] - theirs
                    missing = theirs - ours[f.name]
                    fmt = lambda c: ", ".join(f"{names_at.get(a, hex(a))}x{n}" for a, n in c.items())
                    print(f"{obj.relative_to(ROOT)}: {f.name}: calls {fmt(extra) or '-'}; the original calls "
                          f"{fmt(missing) or '-'}")
                    problems += 1
    print(f"{problems} function(s) with different callees")


if __name__ == "__main__":
    main()
