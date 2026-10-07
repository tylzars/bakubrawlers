#!/usr/bin/env python3
"""Checks that calls in the compiled objects go where the original's calls go.

Function matching masks relocated words, so a function can "match" while calling the wrong function (e.g. `_fdiv`
where the original calls `_fmul`; both are a plain `bl`). For every compiled function whose address is known from
symbols.txt, this decodes each branch the original makes at the same offset and compares it with the address of
the symbol the object calls (when that symbol's address is known). Run after `ninja` and `sync_symbols.py`.

Only fully matching functions are checked (from build/usa/report.json; run `ninja report` first).

Usage: check_calls.py [object.o ...]   (default: every object under build/usa/src)
"""
import json
import re
import struct
import sys
from pathlib import Path

from elftools.elf.elffile import ELFFile

sys.path.insert(0, str(Path(__file__).parent))
from probe_compilers import module_bytes  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
LINE = re.compile(r"^(\S+) kind:(\S+).* addr:(0x[0-9a-f]+)")


def load_symbols() -> dict[str, dict[str, int]]:
    tables = {}
    for path in [ROOT / "config/usa/arm9/symbols.txt", *sorted((ROOT / "config/usa/arm9/overlays").glob("*/symbols.txt"))]:
        module = "main" if path.parent.name == "arm9" else path.parent.name
        table = {}
        for line in path.read_text().splitlines():
            if m := LINE.match(line):
                table.setdefault(m.group(1), int(m.group(3), 16))
        tables[module] = table
    return tables


def module_of(obj: Path) -> str | None:
    rel = obj.relative_to(ROOT / "build/usa").with_suffix(".cpp").as_posix()
    for path in [ROOT / "config/usa/arm9/delinks.txt", *sorted((ROOT / "config/usa/arm9/overlays").glob("*/delinks.txt"))]:
        if f"{rel}:" in path.read_text():
            return "main" if path.parent.name == "arm9" else path.parent.name
    return None


def matched_functions() -> set[str]:
    """Functions objdiff reports as fully matching (only these have code at the same offsets)."""
    report = ROOT / "build/usa/report.json"
    if not report.exists():
        return set()
    names = set()
    for unit in json.loads(report.read_text())["units"]:
        for fn in unit.get("functions", []):
            if fn.get("fuzzy_match_percent", 0) >= 100:
                names.add(fn["name"])
    return names


def main():
    tables = load_symbols()
    matched = matched_functions()
    objs = [Path(p).resolve() for p in sys.argv[1:]] or sorted((ROOT / "build/usa/src").rglob("*.o"))
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
            for sec in elf.iter_sections():
                if not sec.name.startswith(".rel"):
                    continue
                funcs = [s for s in syms if s["st_info"]["type"] == "STT_FUNC" and s["st_shndx"] == sec["sh_info"]
                         and s["st_size"] and s.name in local and s.name in matched]
                for r in sec.iter_relocations():
                    if r["r_info_type"] not in (1, 28, 29):
                        continue
                    fn = next((s for s in funcs if s["st_value"] <= r["r_offset"] < s["st_value"] + s["st_size"]), None)
                    if fn is None:
                        continue
                    target = syms[r["r_info_sym"]].name
                    want = local.get(target, tables["main"].get(target))
                    if want is None:
                        continue
                    site = local[fn.name] + r["r_offset"] - fn["st_value"]
                    if not base <= site < base + len(blob):
                        continue
                    word = struct.unpack_from("<I", blob, site - base)[0]
                    if word & 0x0E000000 != 0x0A000000:  # not a branch in the original
                        continue
                    disp = word & 0xFFFFFF
                    if disp & 0x800000:
                        disp -= 1 << 24
                    actual = site + 8 + disp * 4
                    if actual != want:
                        names = [n for t in tables.values() for n, a in t.items() if a == actual]
                        print(f"{obj.relative_to(ROOT)}: {fn.name}+{r['r_offset'] - fn['st_value']:#x} calls {target} "
                              f"({want:#010x}); the original calls {actual:#010x} {names[:1]}")
                        problems += 1
    print(f"{problems} call(s) differ")
    sys.exit(1 if problems else 0)


if __name__ == "__main__":
    main()
