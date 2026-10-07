#!/usr/bin/env python3
"""Prints C++ initializers for window record tables (WindowCellRecord / WindowItemRecord, 0x1c bytes each).

Reads the extracted module from extract/usa/ so values are exact.

  gen_window_tables.py ov004 cell 0x0221d830 2 sCells
  gen_window_tables.py ov004 item 0x0221d7f8 2 sItems
"""
import struct
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).parent))
from probe_compilers import module_bytes  # noqa: E402

module, kind, addr, count, name = sys.argv[1], sys.argv[2], int(sys.argv[3], 16), int(sys.argv[4]), sys.argv[5]
base, blob = module_bytes(module)


def string_at(ptr: int) -> str:
    return blob[ptr - base:].split(b"\0")[0].decode()


def cell(o: int) -> str:
    path, a, b, c, d, e = struct.unpack_from("<6I", blob, o - base)
    tail = ", ".join(str(v) for v in blob[o - base + 0x18:o - base + 0x1c])
    path_text = f'"{string_at(path)}"' if path else "NULL"
    return f"{{{path_text}, {a}, {b:#x}, {c}, {d}, {e}, {{{tail}}}}}"


def item(o: int) -> str:
    i, a, x, y, w, h, u10 = struct.unpack_from("<2I4hH", blob, o - base)
    nb = struct.unpack_from("<4b", blob, o - base + 0x12)
    u16, u18 = struct.unpack_from("<HI", blob, o - base + 0x16)
    return f"{{{i}, {a:#x}, {x}, {y}, {w}, {h}, {u10}, {{{', '.join(map(str, nb))}}}, {u16}, {u18}}}"


fn, ctype = (cell, "WindowCellRecord") if kind == "cell" else (item, "WindowItemRecord")
print(f"static const {ctype} {name}[{count}] = {{")
for k in range(count):
    print(f"    {fn(addr + k * 0x1c)},")
print("};")
