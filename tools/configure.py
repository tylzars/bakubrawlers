#!/usr/bin/env python3
"""Generates build.ninja for the Bakugan: Battle Brawlers (DS) decomp.

Pipeline:  baserom --dsd rom extract--> extract/<ver>/
           dsd delink                -> build/<ver>/delinks/*.o   (asm-equivalent objects for not-yet-decompiled code)
           mwccarm src/**.c(pp)      -> build/<ver>/src/**.o      (decompiled code, for TUs marked `complete`)
           dsd lcf + mwldarm         -> build/<ver>/arm9.o + module binaries
           dsd rom config/build      -> build/<ver>/bakugan_<ver>.nds, checked against config/<ver>/rom.sha1
"""
import argparse
import json
import os
import shlex
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
os.chdir(ROOT)

GAME = "bakugan"
VERSIONS = {
    "usa": {
        "baserom": f"extract/baserom_{GAME}_usa.nds",
        # See tools/fix_header.py; only used when no ARM7 BIOS is supplied.
        "secure_area_crc": 0x31A7,
    },
}

WIBO_TAG = "1.2.0"
OBJDIFF_TAG = "v3.8.2"
# Compiler and flags are pinned by evidence in docs/compiler.md.
# Game code: CodeWarrior for DS 2.0 sp2p2+ (sp2p2/sp2p3/sp2p4 are codegen-identical on C++).
MWCC_VERSION = "2.0/sp2p2"
DECOMP_ME_COMPILER = "mwcc_30_137"  # decomp.me id for MWCC_VERSION
# NitroSDK was prebuilt by Nintendo with an older compiler (2.0 sp1..sp1p5); sources
# under these paths compile with it instead.
PATH_COMPILERS = {
    "libs/nds/": "2.0/sp1p5",
    "libs/nns/": "2.0/sp1p5",  # NitroSystem: every 2.0 build gives the same code (docs/compiler.md)
}
# Flags those builds don't use. NitroSDK matches with the default inlining (docs/compiler.md).
PATH_DROP_FLAGS = {
    "libs/nds/": ["-inline on,noauto"],
    "libs/nns/": ["-inline on,noauto"],
}

CC_FLAGS = [
    "-O4,p",            # verified: -O3,p differs in regalloc; ,s and -O2 don't inline accessors
    "-ipa file",        # verified: inline-function statics are shared across modules (docs/compiler.md)
    "-inline on,noauto",  # verified: auto would inline same-TU calls the game keeps out of line
    "-enum int",        # verified: FSFile field offsets in game code (docs/compiler.md)
    "-char signed",     # verified: plain-char table walk in game code loads with ldrsb
    "-str noreuse",     # verified: identical literals in one initializer are separate copies
    "-proc arm946e",    # verified: arm9tdmi lacks smulbb
    "-gccext,on",
    "-fp soft",
    "-Cpp_exceptions off",  # verified: on changes new-expression codegen
    "-RTTI off",        # verified: vtables have no typeinfo pointer
    "-interworking",    # verified: leaf returns are bx lr, never mov pc, lr
    "-w off",
    "-sym on",
    "-gccinc",          # treat #include "" and <> alike
    "-nolink",
    "-msgstyle gcc",
]
LD_FLAGS = [
    "-proc arm946e",
    "-dead",
    "-nostdlib",
    "-interworking",
    "-map closure,unused",
    "-msgstyle gcc",
    "-m Entry",
]

parser = argparse.ArgumentParser()
parser.add_argument("version", nargs="?", default="usa", choices=VERSIONS)
parser.add_argument("-w", "--wine", default="tools/bin/wibo", help="wibo or wine binary used to run mwcc/mwld")
parser.add_argument("--compiler", type=Path, help="pre-installed compiler root (contains 2.0/sp2p2/...)")
parser.add_argument("--dsd", type=Path, default=Path("tools/bin/dsd"), help="patched dsd (see tools/build_dsd.sh)")
parser.add_argument("--arm7-bios", type=Path, default=Path("arm7_bios.bin"))
args = parser.parse_args()

ver = args.version
info = VERSIONS[ver]
PY = shlex.quote(sys.executable)
DSD = str(args.dsd)
WINE = args.wine
OBJDIFF = "tools/bin/objdiff-cli"
MWCC_ROOT = args.compiler or Path("tools/mwccarm")
CC = str(MWCC_ROOT / MWCC_VERSION / "mwccarm.exe")
LD = str(MWCC_ROOT / MWCC_VERSION / "mwldarm.exe")

baserom = Path(info["baserom"])
extract_dir = Path("extract") / ver
extract_cfg = extract_dir / "config.yaml"
config_dir = Path("config") / ver / "arm9"
dsd_config = config_dir / "config.yaml"
build_dir = Path("build") / ver
have_bios = args.arm7_bios.is_file()
bios_flag = f"--arm7-bios {args.arm7_bios}" if have_bios else ""

if not args.dsd.is_file():
    sys.exit(f"{args.dsd} not found; run tools/build_dsd.sh first (stock dsd releases can't rebuild this ROM)")
if not baserom.is_file():
    sys.exit(f"{baserom} not found; see extract/README.md")

# dsd needs an extracted ROM to answer `json delinks`, so extract eagerly the first time.
if not extract_cfg.is_file():
    subprocess.run([DSD, "rom", "extract", "--rom", str(baserom), "--output-path", str(extract_dir)]
                   + (["--arm7-bios", str(args.arm7_bios)] if have_bios else []), check=True)
delinks = json.loads(subprocess.run([DSD, "json", "delinks", "-c", str(dsd_config)],
                                    check=True, capture_output=True, text=True).stdout)

dsd_config_files = sorted(str(p) for p in config_dir.rglob("*") if p.suffix in (".txt", ".yaml"))
include_dirs = ["include"] + sorted(str(p) for p in Path("libs").glob("*/include"))
sources = sorted(p for d in ("src", "libs") for p in Path(d).rglob("*") if p.suffix in (".c", ".cpp"))

lines: list[str] = []
w = lines.append


def build(outputs, rule, inputs=(), implicit=(), order_only=(), variables=None):
    outs = " ".join(outputs) if isinstance(outputs, list) else outputs
    ins = " ".join(inputs) if isinstance(inputs, list) else inputs
    s = f"build {outs}: {rule}"
    if ins:
        s += f" {ins}"
    if implicit:
        s += " | " + " ".join(implicit)
    if order_only:
        s += " || " + " ".join(order_only)
    w(s)
    for k, v in (variables or {}).items():
        w(f"  {k} = {v}")
    w("")


w(f"# Generated by tools/configure.py {ver}; do not edit.")
w("ninja_required_version = 1.10")
w("")

# ---- rules ----
w(f"rule download_tool\n  command = {PY} tools/download_tool.py $tool $tag --path $path\n  description = DOWNLOAD $tool\n")
w(f"rule extract\n  command = {DSD} rom extract --rom $in --output-path $outdir {bios_flag}\n  description = EXTRACT $in\n")
w(f"rule delink\n  command = {DSD} delink -c $config && {PY} tools/delink_multidef.py $out\n  description = DELINK\n")
w(f"rule lcf\n  command = {DSD} lcf -c $config && {PY} tools/lcf_force_active.py $lcf_raw $lcf_final --config-dir {config_dir}\n  description = LCF\n")
w(f"rule mwcc\n  command = {WINE} $cc $base_flags $cc_flags {' '.join('-i ' + d for d in include_dirs)} "
  f"-d version_{ver.upper()} -MD -c $in -o $outdir && {PY} tools/transform_dep.py $basefile.d $basefile.d && {PY} tools/demote_multidef.py $out\n"
  f"  depfile = $basefile.d\n  deps = gcc\n  description = MWCC $in\n")
w(f"rule mwld\n  command = {WINE} {LD} {' '.join(LD_FLAGS)} @$objects $lcf -o $out\n  description = LINK $out\n")
w(f"rule disassemble\n  command = {DSD} dis -c $config -a $outdir --ual && touch $out\n  description = DIS\n")
w(f"rule rom_config\n  command = {DSD} rom config --elf $in --config $config\n  description = ROM CONFIG\n")
w(f"rule rom_build\n  command = {DSD} rom build --config $in --rom $out {bios_flag}\n  description = ROM BUILD $out\n")
w(f"rule fix_header\n  command = {PY} tools/fix_header.py $in $out --secure-area-crc $crc\n  description = FIX HEADER $out\n")
w(f"rule sha1\n  command = sha1sum -c $sha1_file && touch $out\n  description = SHA1 $in\n")
w(f"rule objdiff_config\n  command = {DSD} objdiff -c $config -o . --scratch --compiler {DECOMP_ME_COMPILER} "
  f"--c-flags {shlex.quote(' '.join(CC_FLAGS + ['-lang=c++']))} --custom-make ninja\n  description = OBJDIFF CONFIG\n")
w(f"rule objdiff_report\n  command = {OBJDIFF} report generate -o $out\n  description = REPORT\n")
w(f"rule check_symbols\n  command = {DSD} check symbols -c $config --elf-path $in --fail --max-lines 20 && touch $out\n  description = CHECK SYMBOLS\n")
w(f"rule configure\n  command = {PY} tools/configure.py {' '.join(shlex.quote(a) for a in sys.argv[1:])}\n  generator = 1\n  description = CONFIGURE\n")

# ---- tools ----
tool_outputs = []
if WINE == "tools/bin/wibo":
    build(WINE, "download_tool", variables={"tool": "wibo", "tag": WIBO_TAG, "path": WINE})
    tool_outputs.append(WINE)
if args.compiler is None:
    # One zip holds every version, including the PATH_COMPILERS ones.
    extra = [str(MWCC_ROOT / v / "mwccarm.exe") for v in sorted(set(PATH_COMPILERS.values()))]
    build([CC, LD] + extra, "download_tool", variables={"tool": "mwccarm", "tag": "latest", "path": "tools"})
build(OBJDIFF, "download_tool", variables={"tool": "objdiff", "tag": OBJDIFF_TAG, "path": OBJDIFF})

# ---- extract / delink / lcf ----
build(str(extract_cfg), "extract", [str(baserom)], implicit=[DSD], variables={"outdir": str(extract_dir)})

delink_files = [f["delink_file"] for f in delinks["files"]]
build(delink_files, "delink", dsd_config_files + [str(extract_cfg)], implicit=[DSD, "tools/delink_multidef.py"],
      variables={"config": str(dsd_config)})
build("delink", "phony", delink_files)

lcf_raw = delinks["arm9_lcf_file"]
lcf_final = str(Path(lcf_raw).with_name("arm9_final.lcf"))
build([lcf_raw, delinks["arm9_objects_file"], lcf_final], "lcf", dsd_config_files + [str(extract_cfg)],
      implicit=[DSD, "tools/lcf_force_active.py"],
      variables={"config": str(dsd_config), "lcf_raw": lcf_raw, "lcf_final": lcf_final})

# ---- compile ----
# The ROM links the delinked originals until a unit is complete; the compiled objects feed objdiff (report).
compiled_objects = []
for src in sources:
    obj = build_dir / src.with_suffix(".o")
    compiled_objects.append(str(obj))
    cc = next((str(MWCC_ROOT / v / "mwccarm.exe") for prefix, v in PATH_COMPILERS.items()
               if str(src).startswith(prefix)), CC)
    drop = next((f for prefix, f in PATH_DROP_FLAGS.items() if str(src).startswith(prefix)), [])
    build(str(obj), "mwcc", [str(src)], implicit=[cc, "tools/demote_multidef.py"] + tool_outputs, variables={
        "cc": cc,
        "base_flags": " ".join(f for f in CC_FLAGS if f not in drop),
        "cc_flags": "-lang=c++" if src.suffix == ".cpp" else "-lang=c",
        "outdir": str(obj.parent),
        "basefile": str(obj.with_suffix("")),
    })

# ---- link / rom ----
arm9_o = str(build_dir / "arm9.o")
objects = [f["object_to_link"] for f in delinks["files"]]
build(arm9_o, "mwld", objects + [lcf_final], implicit=[LD, delinks["arm9_objects_file"]] + tool_outputs,
      variables={"objects": delinks["arm9_objects_file"], "lcf": lcf_final})
build("arm9", "phony", [arm9_o])

rom_cfg = str(build_dir / "build" / "rom_config.yaml")
build(rom_cfg, "rom_config", [arm9_o], implicit=[DSD], variables={"config": str(dsd_config)})

rom = str(build_dir / f"{GAME}_{ver}.nds")
if have_bios:
    build(rom, "rom_build", [rom_cfg], implicit=[DSD])
else:
    raw_rom = str(build_dir / f"{GAME}_{ver}.nobios.nds")
    build(raw_rom, "rom_build", [rom_cfg], implicit=[DSD])
    build(rom, "fix_header", [raw_rom], implicit=["tools/fix_header.py"], variables={"crc": hex(info["secure_area_crc"])})
build("rom", "phony", [rom])

sha1_file = f"{GAME}_{ver}.sha1"
build(str(build_dir / "rom.ok"), "sha1", [rom], implicit=[sha1_file], variables={"sha1_file": sha1_file})
build("sha1", "phony", [str(build_dir / "rom.ok")])

build(str(build_dir / "symbols.ok"), "check_symbols", [arm9_o], implicit=[DSD], variables={"config": str(dsd_config)})
build("check", "phony", [str(build_dir / "rom.ok"), str(build_dir / "symbols.ok")])

# Disassembly for reading (not committed): build/<ver>/asm/
build(str(build_dir / "asm" / ".stamp"), "disassemble", dsd_config_files, implicit=[DSD],
      variables={"config": str(dsd_config), "outdir": str(build_dir / "asm")})
build("dis", "phony", [str(build_dir / "asm" / ".stamp")])

# ---- objdiff ----
build("objdiff.json", "objdiff_config", dsd_config_files, implicit=[DSD], variables={"config": str(dsd_config)})
build("objdiff", "phony", ["objdiff.json"] + delink_files + compiled_objects)
build(str(build_dir / "report.json"), "objdiff_report", ["objdiff.json"] + delink_files + compiled_objects + [arm9_o],
      implicit=[OBJDIFF])
build("report", "phony", [str(build_dir / "report.json")])

# Re-run configure when TU lists change (new files in delinks.txt or new sources).
build("build.ninja", "configure", implicit=["tools/configure.py"] + dsd_config_files)

w("default sha1")
Path("build.ninja").write_text("\n".join(lines) + "\n")
print(f"Wrote build.ninja for '{ver}' ({len(objects)} objects, {len(sources)} sources, "
      f"{'with' if have_bios else 'without'} ARM7 BIOS)")
