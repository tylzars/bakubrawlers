#!/usr/bin/env python3
"""Searches the order of source lines marked `//PERM` for the best match of one function.

Register allocation and stack-slot assignment in mwcc follow the order in which variables are declared, so a
function whose only differences are register names often matches after its declarations are reordered. Mark each
line that may move with a trailing `//PERM`; lines move only among the other marked lines. A marked declaration can
also list alternative types to try, `//PERM T=u8|u32|int`: its leading type is replaced by each in turn (keep a cast
in the initializer, e.g. `u8 id = (u8) x; //PERM T=u8|u32|int`, so the value stays the same). With up to 8 marked
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
        elif re.match(r"\s+2\.0/sp2p2\s+1/1(\s|$)", line):
            return 0  # every probed function matches (the probe prints no name then)
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
    idx = [i for i, l in enumerate(lines) if re.search(r"//PERM(\s+T=[\w|]+)?\s*$", l)]
    if len(idx) < 2:
        sys.exit("mark at least two lines with //PERM")
    marked = [lines[i] for i in idx]
    # alternative spellings of each marked line (type variants)
    alts = []
    for l in marked:
        m = re.search(r"//PERM\s+T=([\w|]+)\s*$", l)
        if not m:
            alts.append([l])
            continue
        body = l[:m.start()].rstrip() + " //PERM"
        t = re.match(r"^(\s*)(?:const\s+)?(\w+)", body)
        alts.append([body[:t.start(2)] + ty + body[t.end(2):] for ty in m.group(1).split("|")])

    def build(state):
        order, var = state
        new = list(lines)
        for k, i in enumerate(idx):
            new[i] = alts[order[k]][var[order[k]]]
        return "\n".join(new)

    workdir = ROOT / "build/probe"
    workdir.mkdir(parents=True, exist_ok=True)
    base = (tuple(range(len(marked))), tuple(0 for _ in marked))
    best, best_score = base, score(build(base), a.function, workdir)
    print(f"start: {best_score} differing instructions", flush=True)
    if best_score == 10**6:
        sys.exit(f"function '{a.function}' not found in the probe output")

    combos = math.prod(len(x) for x in alts)
    with ThreadPoolExecutor(a.jobs) as pool:
        if math.factorial(len(marked)) * combos <= 40320:
            states = [(o, v) for v in itertools.product(*[range(len(x)) for x in alts])
                      for o in itertools.permutations(base[0])]
            for k in range(0, len(states), 256):
                chunk = states[k:k + 256]
                for st, s in zip(chunk, pool.map(lambda o: score(build(o), a.function, workdir), chunk)):
                    if s < best_score:
                        best, best_score = st, s
                        print(f"{s}", flush=True)
                if best_score == 0:
                    break
        else:
            rng = random.Random(a.seed)
            for _ in range(0, a.iters, a.jobs):
                cands = []
                for _ in range(a.jobs):
                    order, var = list(best[0]), list(best[1])
                    for _ in range(rng.choice([1, 1, 2])):
                        typed = [i for i, x in enumerate(alts) if len(x) > 1]
                        if typed and rng.random() < 0.3:
                            i = rng.choice(typed)
                            var[i] = rng.randrange(len(alts[i]))
                        else:
                            i, j = rng.sample(range(len(order)), 2)
                            order.insert(j, order.pop(i))
                    cands.append((tuple(order), tuple(var)))
                for st, s in zip(cands, pool.map(lambda o: score(build(o), a.function, workdir), cands)):
                    if s <= best_score:
                        if s < best_score:
                            print(f"{s}", flush=True)
                        best, best_score = st, s
                if best_score == 0:
                    break

    out = a.source.with_suffix(".best.cpp")
    out.write_text(build(best))
    print(f"best: {best_score} differing instructions -> {out}" + ("  (MATCH)" if best_score == 0 else ""))


if __name__ == "__main__":
    main()
