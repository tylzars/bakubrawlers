#!/usr/bin/env python3
"""Random source mutation search for one near-matching function (register allocation and scheduling diffs).

Mark the region to mutate inside the function with lines `// MUTATE-BEGIN` and `// MUTATE-END`; put one statement
or declaration per line there. The tool hill-climbs over random edits of that region, scoring each variant with
tools/probe_compilers.py (number of differing instructions, plus a penalty for a size difference):
  * swap two adjacent lines that are simple statements or declarations,
  * split `T x = e;` into `T x;` + `x = e;` (and merge them back),
  * change the type of a local (int, s32, u32, u16, s16, u8, s8),
  * swap the operands of a commutative operator,
  * add or drop an integer cast around an identifier,
  * rewrite `x++` / `x += 1` / `x = x + 1` (and the same for --),
  * replace a pointer local (`T *p = e;`) by its initializer at each use and drop the declaration.
Only byte-identical output counts as a match, so an edit that changes the meaning can only "win" if the original
compiled to the same thing. Review the result anyway (statement swaps can move side effects).

  mutate.py src.cpp [--jobs 8] [--minutes 10] [--seed N]

The source needs the usual `// PROBE:` line. The best variant is written to <src>.best.cpp whenever it improves.
"""
import argparse
import os
import random
import re
import subprocess
import sys
import tempfile
import time
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
FLAGS = "-O4,p -ipa file -inline on,noauto"
TYPES = ["int", "s32", "u32", "u16", "s16", "u8", "s8"]
DECL = re.compile(r"^(\s*)(const\s+)?(int|s32|u32|u16|s16|u8|s8|BOOL|unsigned int|signed int)\s+(\w+)\s*(=\s*(.+))?;\s*$")
SIMPLE = re.compile(r"^\s*[^{}]*;\s*(//.*)?$")
COMM = re.compile(r"(\b[\w.\[\]>-]+\b)\s*([+*&|^]|==|!=)\s*(\b[\w.\[\]>-]+\b)")


def score(text, workdir, opts):
    with tempfile.NamedTemporaryFile("w", suffix=".cpp", dir=workdir, delete=False) as f:
        f.write(text)
        path = f.name
    try:
        out = subprocess.run([sys.executable, str(ROOT / "tools/probe_compilers.py"), path, "--only", opts.compiler,
                              f"--flags={opts.flags}", "--show"], cwd=ROOT, capture_output=True, text=True).stdout
    finally:
        os.unlink(path)
    if not out.strip() or "COMPILE ERROR" in out:
        return 10 ** 6
    if " 1/1 " in out or out.rstrip().endswith("1/1"):
        return 0
    n = sum(1 for ln in out.splitlines() if re.match(r"\s+[0-9a-f]+ \*", ln))
    m = re.search(r"size (0x[0-9a-f]+) vs (0x[0-9a-f]+)", out)
    if m:
        n += 1000 + abs(int(m.group(1), 16) - int(m.group(2), 16))
    if not out.strip() or "error" in out.lower() and n == 0:
        return 10 ** 6
    return n


def mutate(lines, rng):
    out = list(lines)
    for _ in range(rng.choice([1, 1, 1, 2, 3])):
        kind = rng.randrange(7)
        idx = list(range(len(out)))
        if kind == 0:  # swap adjacent simple lines
            cand = [i for i in idx[:-1] if SIMPLE.match(out[i]) and SIMPLE.match(out[i + 1])]
            if cand:
                i = rng.choice(cand)
                out[i], out[i + 1] = out[i + 1], out[i]
        elif kind == 1:  # split / merge declaration and initialisation
            cand = [i for i in idx if (m := DECL.match(out[i])) and m.group(6)]
            merge = [i for i in idx[:-1] if (m := DECL.match(out[i])) and not m.group(6)
                     and re.match(r"^\s*" + m.group(4) + r"\s*=\s*.+;\s*$", out[i + 1])]
            if cand and (not merge or rng.random() < 0.5):
                i = rng.choice(cand)
                m = DECL.match(out[i])
                out[i:i + 1] = [f"{m.group(1)}{m.group(2) or ''}{m.group(3)} {m.group(4)};",
                                f"{m.group(1)}{m.group(4)} = {m.group(6)};"]
            elif merge:
                i = rng.choice(merge)
                m = DECL.match(out[i])
                rhs = out[i + 1].split("=", 1)[1].strip().rstrip(";")
                out[i:i + 2] = [f"{m.group(1)}{m.group(2) or ''}{m.group(3)} {m.group(4)} = {rhs};"]
        elif kind == 2:  # retype a local
            cand = [i for i in idx if DECL.match(out[i])]
            if cand:
                i = rng.choice(cand)
                m = DECL.match(out[i])
                t = rng.choice([x for x in TYPES if x != m.group(3)])
                out[i] = out[i][:m.start(3)] + t + out[i][m.end(3):]
        elif kind == 3:  # swap commutative operands
            cand = [i for i in idx if COMM.search(out[i]) and not out[i].lstrip().startswith("//")]
            if cand:
                i = rng.choice(cand)
                ms = list(COMM.finditer(out[i]))
                m = rng.choice(ms)
                out[i] = out[i][:m.start()] + f"{m.group(3)} {m.group(2)} {m.group(1)}" + out[i][m.end():]
        elif kind == 5:  # increment forms
            cand = [i for i in idx if re.search(r"(\b[\w.\[\]>-]+)(\+\+|--|\s*[+-]=\s*1\b);", out[i])]
            if cand:
                i = rng.choice(cand)
                m = re.search(r"(\b[\w.\[\]>-]+)(\+\+|--|\s*([+-])=\s*1\b);", out[i])
                v, sign = m.group(1), "+" if ("+" in m.group(2)) else "-"
                forms = [f"{v}{sign}{sign};", f"{v} {sign}= 1;", f"{v} = {v} {sign} 1;"]
                out[i] = out[i][:m.start()] + rng.choice(forms) + out[i][m.end():]
        elif kind == 6:  # inline a pointer local
            cand = [i for i in idx if re.match(r"^\s*(?:const\s+)?\w+\s*\*\s*(\w+)\s*=\s*([^;]+);\s*$", out[i])]
            if cand:
                i = rng.choice(cand)
                m = re.match(r"^\s*(?:const\s+)?\w+\s*\*\s*(\w+)\s*=\s*([^;]+);\s*$", out[i])
                name, init = m.group(1), m.group(2).strip()
                rest = [re.sub(r"\b" + name + r"\b", f"({init})", l) for l in out[i + 1:]]
                out = out[:i] + rest
        else:  # add or drop a cast
            cand = [i for i in idx if SIMPLE.match(out[i])]
            if cand:
                i = rng.choice(cand)
                casts = list(re.finditer(r"\((?:u8|s8|u16|s16|u32|s32|int)\)\s*", out[i]))
                if casts and rng.random() < 0.5:
                    c = rng.choice(casts)
                    out[i] = out[i][:c.start()] + out[i][c.end():]
                else:
                    ids = [m for m in re.finditer(r"(?<![\w.>])([a-z_]\w*)(?![\w(])", out[i])
                           if m.group(1) not in ("return", "if", "for", "while", "do", "else") + tuple(TYPES)]
                    if ids:
                        m = rng.choice(ids)
                        out[i] = out[i][:m.start()] + f"({rng.choice(TYPES)}) " + out[i][m.start():]
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("src")
    ap.add_argument("--jobs", type=int, default=8)
    ap.add_argument("--minutes", type=float, default=10)
    ap.add_argument("--seed", type=int, default=None)
    ap.add_argument("--compiler", default="2.0/sp2p2")
    ap.add_argument("--flags", default=FLAGS)
    opts = ap.parse_args()
    text = Path(opts.src).read_text()
    lines = text.split("\n")
    try:
        b = next(i for i, l in enumerate(lines) if l.strip() == "// MUTATE-BEGIN")
        e = next(i for i, l in enumerate(lines) if l.strip() == "// MUTATE-END")
    except StopIteration:
        sys.exit("mark the region with // MUTATE-BEGIN and // MUTATE-END lines")
    head, region, tail = lines[:b + 1], lines[b + 1:e], lines[e:]
    rng = random.Random(opts.seed)
    work = Path(tempfile.mkdtemp(prefix="mutate_", dir=ROOT / "build"))
    best_region, best = region, score(text, work, opts)
    print(f"start: {best}", flush=True)
    deadline = time.time() + opts.minutes * 60
    cur, cur_score = region, best
    with ThreadPoolExecutor(opts.jobs) as ex:
        while time.time() < deadline and best > 0:
            cands = [mutate(cur, rng) for _ in range(opts.jobs)]
            scores = list(ex.map(lambda r: score("\n".join(head + r + tail), work, opts), cands))
            for r, s in zip(cands, scores):
                if s < best:
                    best, best_region = s, r
                    Path(opts.src + ".best.cpp").write_text("\n".join(head + r + tail))
                    print(f"{time.strftime('%H:%M:%S')} best {s}", flush=True)
                if s <= cur_score or rng.random() < 0.05:
                    cur, cur_score = r, s
            if rng.random() < 0.02:
                cur, cur_score = best_region, best
    print(f"done: best {best}" + (" (MATCH)" if best == 0 else ""))


if __name__ == "__main__":
    main()
