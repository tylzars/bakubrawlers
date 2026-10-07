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
    """Returns {symbol: (code bytes, masked word offsets)} for every function in obj."""
    syms = subprocess.run(["arm-none-eabi-readelf", "-sW", str(obj)], capture_output=True, text=True).stdout
    relocs = subprocess.run(["arm-none-eabi-readelf", "-rW", str(obj)], capture_output=True, text=True).stdout
    shdrs = subprocess.run(["arm-none-eabi-readelf", "-SW", str(obj)], capture_output=True, text=True).stdout
    data = obj.read_bytes()

    sections = {}
    for m in re.finditer(r"\[\s*(\d+)\]\s+(\S+)\s+\S+\s+[0-9a-f]+\s+([0-9a-f]+)\s+([0-9a-f]+)", shdrs):
        sections[int(m.group(1))] = (m.group(2), int(m.group(3), 16), int(m.group(4), 16))

    # Relocation offsets per section name.
    masked: dict[str, set[int]] = {}
    cur = None
    for line in relocs.splitlines():
        if m := re.match(r"Relocation section '\.rela?(\S+)'", line):
            cur = masked.setdefault(m.group(1), set())
        elif cur is not None and (m := re.match(r"([0-9a-f]{8})\s", line)):
            cur.add(int(m.group(1), 16) & ~3)

    funcs = {}
    for line in syms.splitlines():
        p = line.split()
        if len(p) >= 8 and p[3] == "FUNC" and p[6].isdigit():
            name, shndx, value, size = p[7], int(p[6]), int(p[1], 16), int(p[2])
            sname, off, _ = sections[shndx]
            code = data[off + value: off + value + size]
            funcs[name] = (code, {r - value for r in masked.get(sname, ()) if value <= r < value + size})
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
