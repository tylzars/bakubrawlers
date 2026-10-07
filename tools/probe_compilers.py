#!/usr/bin/env python3
"""Compiles a probe source with every installed mwccarm version and reports which
ones reproduce the original bytes of the functions it defines.

Each function to check is declared in the source with a marker comment:
    // PROBE: <symbol> <module> <address>
e.g. // PROBE: _ZN5SceneC1Ev ov006 0x02219440

Words covered by relocations in the compiled object are masked (branch targets,
literal pool entries), so only instruction encoding and layout are compared.
"""
import argparse
import re
import struct
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
WIBO = ROOT / "tools/bin/wibo"
MWCC_ROOT = ROOT / "tools/mwccarm"
EXTRACT = ROOT / "extract/us"
BASE_FLAGS = "-proc arm946e -fp soft -interworking -w off -nolink -enum int -char signed -str noreuse -gccext,on"

PROBE = re.compile(r"//\s*PROBE:\s*(\S+)\s+(\S+)\s+(0x[0-9a-fA-F]+)")


def module_bytes(module: str) -> tuple[int, bytes]:
    if module == "main":
        return 0x02000000, (EXTRACT / "arm9/arm9.bin").read_bytes()
    if module == "itcm":
        return 0x01FF8000, (EXTRACT / "arm9/itcm.bin").read_bytes()
    if module.startswith("ov"):
        return 0x02219440, (EXTRACT / f"arm9_overlays/{module}.bin").read_bytes()
    raise SystemExit(f"unknown module {module}")


def orig_size(module: str, addr: int) -> int:
    cfg = ROOT / "config/us/arm9"
    path = cfg / "symbols.txt" if module == "main" else cfg / module / "symbols.txt" if module == "itcm" \
        else cfg / "overlays" / module / "symbols.txt"
    for line in path.read_text().splitlines():
        if f"addr:{addr:#010x}" in line and (m := re.search(r"size=(0x[0-9a-f]+)", line)):
            return int(m.group(1), 16)
    raise SystemExit(f"no function at {addr:#010x} in {path}")


def elf_functions(obj: Path) -> dict[str, tuple[bytes, set[int]]]:
    """Returns {symbol: (code bytes, word offsets covered by that section's relocations)}.

    mwcc emits one .text section per function, so relocations are keyed by the
    section index they apply to (sh_info), never by section name."""
    from elftools.elf.elffile import ELFFile
    from elftools.elf.relocation import RelocationSection

    with open(obj, "rb") as fh:
        elf = ELFFile(fh)
        masked: dict[int, set[int]] = {}
        for sec in elf.iter_sections():
            if isinstance(sec, RelocationSection):
                masked.setdefault(sec["sh_info"], set()).update(r["r_offset"] & ~3 for r in sec.iter_relocations())
        funcs = {}
        symtab = elf.get_section_by_name(".symtab")
        for sym in symtab.iter_symbols():
            if sym["st_info"]["type"] != "STT_FUNC" or not isinstance(sym["st_shndx"], int):
                continue
            data = elf.get_section(sym["st_shndx"]).data()
            value, size = sym["st_value"] & ~1, sym["st_size"]
            code = data[value: value + size]
            mask = {r - value for r in masked.get(sym["st_shndx"], ()) if value <= r < value + size}
            funcs[sym.name] = (code, mask)
        return funcs


def compare(code: bytes, mask: set[int], orig: bytes) -> tuple[bool, str]:
    if len(code) != len(orig):
        return False, f"size {len(code):#x} vs {len(orig):#x}"
    bad = [i for i in range(0, len(code), 4) if i not in mask and code[i:i + 4] != orig[i:i + 4]]
    return (not bad), (f"{len(bad)} words differ (first +{bad[0]:#x})" if bad else "match")


def disasm(code: bytes) -> list[str]:
    tmp = ROOT / "build/probe/_dis.bin"
    tmp.write_bytes(code)
    out = subprocess.run(["arm-none-eabi-objdump", "-D", "-b", "binary", "-m", "arm", str(tmp)],
                         capture_output=True, text=True).stdout
    return [re.sub(r"\s+", " ", l.split("\t", 2)[-1]).strip() if "\t" in l else ""
            for l in out.splitlines() if re.match(r"\s+[0-9a-f]+:\t", l)]


def show_diff(code: bytes, mask: set[int], orig: bytes) -> None:
    a, b = disasm(code), disasm(orig)
    for i in range(max(len(a), len(b))):
        x = a[i] if i < len(a) else ""
        y = b[i] if i < len(b) else ""
        mark = " " if (i * 4 in mask or x == y) else "*"
        print(f"    {i * 4:4x} {mark} {x:40} | {y}")


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("source", type=Path)
    ap.add_argument("--flags", action="append", default=None,
                    help="optimization flag set(s) to try (default: several)")
    ap.add_argument("--only", help="substring filter on compiler dirs")
    ap.add_argument("-v", "--verbose", action="store_true")
    ap.add_argument("--show", action="store_true", help="print a side-by-side disassembly of mismatches")
    a = ap.parse_args()

    probes = [(s, mod, int(addr, 16)) for s, mod, addr in PROBE.findall(a.source.read_text())]
    if not probes:
        raise SystemExit("no // PROBE: lines in source")
    lang = "-lang=c++ -Cpp_exceptions off -RTTI off" if a.source.suffix == ".cpp" else "-lang=c"
    flag_sets = a.flags or ["-O4,p", "-O4,s", "-O3,p", "-O2"]
    compilers = sorted(p.parent for p in MWCC_ROOT.glob("*/*/mwccarm.exe"))
    if a.only:
        compilers = [c for c in compilers if a.only in str(c)]

    out_dir = ROOT / "build/probe"
    out_dir.mkdir(parents=True, exist_ok=True)
    obj = out_dir / (a.source.stem + ".o")

    for flags in flag_sets:
        print(f"== {flags}")
        for cdir in compilers:
            obj.unlink(missing_ok=True)
            cmd = f"{WIBO} {cdir}/mwccarm.exe {BASE_FLAGS} {lang} {flags} -c {a.source} -o {obj}"
            r = subprocess.run(cmd, shell=True, capture_output=True, text=True)
            label = str(cdir.relative_to(MWCC_ROOT))
            if r.returncode or not obj.exists():
                print(f"  {label:12} COMPILE ERROR {r.stdout.strip()[:200]}{r.stderr.strip()[:200]}")
                continue
            funcs = elf_functions(obj)
            results = []
            for sym, mod, addr in probes:
                if sym not in funcs:  # allow a bare or mis-mangled name: match on the identifier
                    ident = re.sub(r"^_Z\d+|^_ZN\d+", "", sym).split("E")[0]
                    cands = [f for f in funcs if ident and ident in f]
                    if len(cands) == 1:
                        sym = cands[0]
                if sym not in funcs:
                    results.append((False, f"{sym} missing (have {', '.join(funcs)})"))
                    continue
                base, blob = module_bytes(mod)
                code, mask = funcs[sym]
                orig = blob[addr - base: addr - base + orig_size(mod, addr)]
                results.append(compare(code, mask, orig))
                if a.show and not results[-1][0]:
                    print(f"  [{cdir.relative_to(MWCC_ROOT)}] {sym}  (compiled | original)")
                    show_diff(code, mask, orig)
            ok = sum(r[0] for r in results)
            detail = "" if ok == len(results) and not a.verbose else "  " + "; ".join(
                f"{p[0]}: {r[1]}" for p, r in zip(probes, results) if a.verbose or not r[0])
            print(f"  {label:12} {ok}/{len(results)}{detail}")


if __name__ == "__main__":
    sys.exit(main())
