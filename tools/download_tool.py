#!/usr/bin/env python3
"""Downloads a third-party build tool into tools/."""
import argparse
import io
import stat
import urllib.request
import zipfile
from pathlib import Path

URLS = {
    "wibo": "https://github.com/decompals/wibo/releases/download/{tag}/wibo-x86_64",
    "objdiff": "https://github.com/encounter/objdiff/releases/download/{tag}/objdiff-cli-linux-x86_64",
    # Metrowerks CodeWarrior for DS compilers, same bundle zeldaret/ph uses
    "mwccarm": "http://decomp.aetias.com/files/mwccarm.zip",
}

parser = argparse.ArgumentParser()
parser.add_argument("tool", choices=URLS)
parser.add_argument("tag")
parser.add_argument("--path", type=Path, required=True, help="output file, or extraction dir for zips")
args = parser.parse_args()

url = URLS[args.tool].format(tag=args.tag)
print(f"Downloading {args.tool} {args.tag} from {url}")
with urllib.request.urlopen(url) as resp:
    data = resp.read()

if url.endswith(".zip"):
    zipfile.ZipFile(io.BytesIO(data)).extractall(args.path)
else:
    args.path.parent.mkdir(parents=True, exist_ok=True)
    args.path.write_bytes(data)
    args.path.chmod(args.path.stat().st_mode | stat.S_IEXEC)
