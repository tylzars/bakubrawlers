#!/usr/bin/env python3
"""Tracks matching progress over time and renders the README graph.

  progress.py update   append the current objdiff report to progress/history.csv
                       (if it changed), redraw progress/progress.svg, rewrite the
                       README section between the progress markers
  progress.py check    exit 1 if the report differs from the last recorded point
                       (used by .githooks/pre-push)

Both read build/<ver>/report.json, so run `ninja report` first.
"""
import argparse
import csv
import json
import sys
from datetime import datetime, timezone
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
HISTORY = ROOT / "progress" / "history.csv"
SVG = ROOT / "progress" / "progress.svg"
README = ROOT / "README.md"
START, END = "<!-- progress:start -->", "<!-- progress:end -->"
FIELDS = ["date", "matched_code", "complete_code", "total_code", "matched_functions", "total_functions",
          "fuzzy_match_percent"]

# (key, label, light, dark): categorical slots 1-2, validated light and dark.
# matched_code: compiled C byte-identical to the original assembly.
# fuzzy_match_percent: objdiff's overall similarity, crediting near-matches too.
SERIES = [
    ("matched_code", "Matched code", "#2a78d6", "#3987e5"),
    ("fuzzy_match_percent", "Overall accuracy", "#eb6834", "#d95926"),
]


def read_report(ver: str) -> dict:
    path = ROOT / "build" / ver / "report.json"
    if not path.is_file():
        sys.exit(f"{path} missing; run `ninja report` first")
    m = json.loads(path.read_text())["measures"]
    row = {k: int(m.get(k, 0)) for k in FIELDS[1:-1]}
    row["fuzzy_match_percent"] = round(float(m.get("fuzzy_match_percent", 0)), 3)
    return row


def read_history() -> list[dict]:
    if not HISTORY.is_file():
        return []
    with HISTORY.open() as f:
        return [{k: v if k == "date" else float(v) if k == "fuzzy_match_percent" else int(v)
                 for k, v in row.items()} for row in csv.DictReader(f)]


def same(a: dict, b: dict) -> bool:
    return all(a[k] == b[k] for k in FIELDS[1:])


def pct(row: dict, key: str) -> float:
    if key == "fuzzy_match_percent":
        return row[key]
    return 100 * row[key] / row["total_code"] if row["total_code"] else 0.0


def nice_ceiling(v: float) -> float:
    for c in (1, 2, 5, 10, 20, 25, 50, 75, 100):
        if v <= c:
            return c
    return 100


def _theme_css(light: bool) -> str:
    surface, text, muted, grid = ("#fcfcfb", "#52514e", "#898781", "#e1e0d9") if light else \
                                 ("#1a1a19", "#c3c2b7", "#898781", "#2c2c2a")
    css = f".bg{{fill:{surface}}}.grid{{stroke:{grid}}}.muted{{fill:{muted}}}.lbl{{fill:{text}}}"
    for i, (_, _, lc, dc) in enumerate(SERIES):
        c = lc if light else dc
        css += f".ln{i}{{stroke:{c}}}.dot{i}{{fill:{c};stroke:{surface}}}"
    return css


def render_svg(rows: list[dict]) -> str:
    W, H = 720, 300
    L, R, T, B = 48, 176, 24, 36
    pw, ph = W - L - R, H - T - B
    times = [datetime.fromisoformat(r["date"]).timestamp() for r in rows]
    t0, t1 = min(times), max(times)
    ymax = nice_ceiling(max(pct(r, k) for r in rows for k, *_ in SERIES))

    def x(t):
        return L + (pw / 2 if t1 == t0 else (t - t0) / (t1 - t0) * pw)

    def y(v):
        return T + ph - v / ymax * ph

    out = [
        f'<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 {W} {H}" width="{W}" height="{H}" '
        f'role="img" aria-label="Decompilation progress over time">',
        "<style>",
        _theme_css(light=True),
        "@media (prefers-color-scheme: dark){" + _theme_css(light=False) + "}",
        "text{font:12px -apple-system,BlinkMacSystemFont,'Segoe UI',Helvetica,Arial,sans-serif}",
        "</style>",
        f'<rect class="bg" width="{W}" height="{H}" rx="6"/>',
    ]
    for i in range(5):
        v = ymax * i / 4
        out.append(f'<line x1="{L}" x2="{L + pw}" y1="{y(v):.1f}" y2="{y(v):.1f}" class="grid" stroke-width="1"/>')
        out.append(f'<text x="{L - 8}" y="{y(v) + 4:.1f}" class="muted" text-anchor="end">{v:g}%</text>')
    for t, anchor in ((t0, "start"), (t1, "end")) if t1 != t0 else ((t0, "middle"),):
        out.append(f'<text class="muted" x="{x(t):.1f}" y="{H - 12}" text-anchor="{anchor}">'
                   f'{datetime.fromtimestamp(t, timezone.utc):%Y-%m-%d}</text>')

    # Lines first, then end dots with surface rings, then direct labels (nudged apart).
    ends = []
    for i, (key, label, *_) in enumerate(SERIES):
        pts = [(x(t), y(pct(r, key))) for t, r in zip(times, rows)]
        if len(pts) > 1:
            d = " ".join(f"{px:.1f},{py:.1f}" for px, py in pts)
            out.append(f'<polyline points="{d}" fill="none" class="ln{i}" stroke-width="2" '
                       f'stroke-linejoin="round" stroke-linecap="round"/>')
        ends.append([pts[-1][1], i, label, pct(rows[-1], key), pts[-1][0]])
    for ey, i, _, _, ex in ends:
        out.append(f'<circle cx="{ex:.1f}" cy="{ey:.1f}" r="4" class="dot{i}" stroke-width="2"/>')
    # Stack labels upward from the lowest so they never collide or drop below the plot.
    ends.sort(reverse=True)
    ends[0][0] = min(ends[0][0], T + ph)
    for j in range(1, len(ends)):
        ends[j][0] = min(ends[j][0], ends[j - 1][0] - 16)
    for ly, i, label, v, _ in ends:
        out.append(f'<line x1="{L + pw + 10}" x2="{L + pw + 22}" y1="{ly:.1f}" y2="{ly:.1f}" class="ln{i}" stroke-width="2"/>')
        out.append(f'<text class="lbl" x="{L + pw + 28}" y="{ly + 4:.1f}">{label} {v:.2f}%</text>')
    out.append("</svg>")
    return "\n".join(out) + "\n"


def readme_section(rows: list[dict]) -> str:
    r = rows[-1]
    fn = f"{r['matched_functions']:,} / {r['total_functions']:,}"
    return "\n".join([
        START,
        "![Decompilation progress over time](progress/progress.svg)",
        "",
        "| Measure | Progress |",
        "|---|---|",
        f"| Matched code | {pct(r, 'matched_code'):.2f}% ({r['matched_code']:,} / {r['total_code']:,} bytes) |",
        f"| Linked code (complete units) | {pct(r, 'complete_code'):.2f}% ({r['complete_code']:,} bytes) |",
        f"| Overall accuracy (fuzzy) | {r['fuzzy_match_percent']:.2f}% |",
        f"| Matched functions | {fn} |",
        "",
        f"_Last updated {r['date'][:10]}. Full history: [`progress/history.csv`](progress/history.csv)._",
        END,
    ])


def main() -> None:
    p = argparse.ArgumentParser()
    p.add_argument("cmd", choices=["update", "check"])
    p.add_argument("--version", default="usa")
    a = p.parse_args()

    cur = read_report(a.version)
    rows = read_history()

    if a.cmd == "check":
        if not rows or not same(rows[-1], cur):
            sys.exit("Progress graph is out of date: run `ninja report && tools/progress.py update` and commit.")
        return

    if not rows or not same(rows[-1], cur):
        rows.append({"date": datetime.now(timezone.utc).isoformat(timespec="seconds"), **cur})
        HISTORY.parent.mkdir(exist_ok=True)
        with HISTORY.open("w", newline="") as f:
            wr = csv.DictWriter(f, FIELDS)
            wr.writeheader()
            wr.writerows(rows)
    SVG.write_text(render_svg(rows))

    text = README.read_text()
    if START in text and END in text:
        text = text[:text.index(START)] + readme_section(rows) + text[text.index(END) + len(END):]
    else:
        text = text.rstrip() + "\n\n## Progress\n" + readme_section(rows) + "\n"
    README.write_text(text)
    print(f"matched {pct(rows[-1], 'matched_code'):.2f}%, overall {rows[-1]['fuzzy_match_percent']:.2f}%, "
          f"linked {pct(rows[-1], 'complete_code'):.2f}% ({len(rows)} points)")


if __name__ == "__main__":
    main()
