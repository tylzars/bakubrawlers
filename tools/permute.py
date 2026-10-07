#!/usr/bin/env python3
"""Searches the order of source lines marked `//PERM` for the best match of one function.

Register allocation and stack-slot assignment in mwcc follow the order in which variables are declared, so a
function whose only differences are register names often matches after its declarations are reordered. Mark each
line that may move with a trailing `//PERM`; lines move only among the other marked lines. With up to 8 marked
lines every order is tried, otherwise the search hill-climbs from the current order.

usage: tools/permute.py src.cpp <function substring> [--iters N] [--jobs N] [--seed N]

The source needs the usual `// PROBE:` lines (see tools/probe_compilers.py); PROBE_INCLUDE is honoured. The best
order is written to <src>.best.cpp. Only reorder lines whose order does not change the meaning of the code
(declarations, independent statements); the score is a byte comparison and cannot tell the difference.
"""
import argparse
import itertools
import math
import os
import random
import re
import subprocess
import sys
import tempfile
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
FLAGS = "-O4,p -ipa file -inline on,noauto"


def score(text: str, func: str, workdir: Path) -> int:
    """Number of differing instructions in the function (0 = match)."""
    with tempfile.NamedTemporaryFile("w", suffix=".cpp", dir=workdir, delete=False) as f:
        f.write(text)
        path = f.name
    try:
        out = subprocess.run([sys.executable, str(ROOT / "tools/probe_compilers.py"), path, "--only", "2.0/sp2p2",
                              f"--flags={FLAGS}", "--show"], cwd=ROOT, capture_output=True, text=True).stdout
    finally:
        os.unlink(path)
    count, inside, found = 0, False, False
    for line in out.split("\n"):
        if line.startswith("  [2.0"):
            inside = func in line
            found |= inside
        elif re.match(r"\s+2\.0/sp2p2\s+1/1\s", line) and func in line:
            return 0
        elif inside and re.match(r"\s+[0-9a-f]+ \* ", line):
            count += 1
    return count if found else 10**6


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("source", type=Path)
    ap.add_argument("function", help="substring of the mangled name in the probe output")
    ap.add_argument("--iters", type=int, default=400, help="hill-climb steps when exhaustive search is too big")
    ap.add_argument("--jobs", type=int, default=os.cpu_count() or 4)
    ap.add_argument("--seed", type=int, default=0)
    a = ap.parse_args()

    lines = a.source.read_text().split("\n")
    idx = [i for i, l in enumerate(lines) if l.rstrip().endswith("//PERM")]
    if len(idx) < 2:
        sys.exit("mark at least two lines with //PERM")
    marked = [lines[i] for i in idx]

    def build(order):
        new = list(lines)
        for k, i in enumerate(idx):
            new[i] = marked[order[k]]
        return "\n".join(new)

    workdir = ROOT / "build/probe"
    workdir.mkdir(parents=True, exist_ok=True)
    base = tuple(range(len(marked)))
    best, best_score = base, score(build(base), a.function, workdir)
    print(f"start: {best_score} differing instructions", flush=True)
    if best_score == 10**6:
        sys.exit(f"function '{a.function}' not found in the probe output")

    with ThreadPoolExecutor(a.jobs) as pool:
        if math.factorial(len(marked)) <= 40320:
            orders = list(itertools.permutations(base))
            for order, s in zip(orders, pool.map(lambda o: score(build(o), a.function, workdir), orders)):
                if s < best_score:
                    best, best_score = order, s
                    print(f"{s}", flush=True)
        else:
            rng = random.Random(a.seed)
            for _ in range(0, a.iters, a.jobs):
                cands = []
                for _ in range(a.jobs):
                    c = list(best)
                    for _ in range(rng.choice([1, 1, 2])):
                        i, j = rng.sample(range(len(c)), 2)
                        c.insert(j, c.pop(i))
                    cands.append(tuple(c))
                for order, s in zip(cands, pool.map(lambda o: score(build(o), a.function, workdir), cands)):
                    if s <= best_score:
                        if s < best_score:
                            print(f"{s}", flush=True)
                        best, best_score = order, s
                if best_score == 0:
                    break

    out = a.source.with_suffix(".best.cpp")
    out.write_text(build(best))
    print(f"best: {best_score} differing instructions -> {out}" + ("  (MATCH)" if best_score == 0 else ""))


if __name__ == "__main__":
    main()
