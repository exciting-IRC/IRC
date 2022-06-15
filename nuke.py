#!/usr/bin/env python3
"""
nuke.py:
    remove files recursively that does not fit in subject.
    secret files are not removed.
    use only on github action!

usage:
    nuke.py [--help] [options] [--execute] [<file>...]

options:
    -h, --help     show this help message and exit
    --execute      actually remove files (not just print)
    <file>         file extension / subdirectory to keep.
                   files can be explicitly specified.
                   [default: ".mk", ".txt", ".cpp", ".hpp", ".tpp", "src/", "Makefile"]
"""
import shutil
from pathlib import Path
from typing import Callable

from flupy import flu

from cpputil import run_docopt


def nuke(
    *,
    exts: list[str],
    explicit: list[str],
    subdirs: list[Path],
    payload: Callable[[Path], None],
):
    for path in Path(__file__).parent.glob("[!.]*"):
        if (
            path.name in explicit
            or path.stem + "/" in subdirs
            or path.suffix in exts
        ):
            continue
        payload(path)


def main():
    args = run_docopt(__doc__)
    # fmt: off
    files = (
        [".mk", ".txt", ".cpp", ".hpp", ".tpp", "src/", "Makefile"]
        + args.get("<file>", [])  # type:ignore
    )
    exts, subdirs, explicit = (
        flu(files).filter(f).collect() for f in (
            lambda x: x.startswith("."),
            lambda x: x.endswith("/"),
            lambda x: not x.startswith(".") and not x.endswith("/"),
        )
    )
    # fmt: on
    print(f"excluded: {exts = }, {explicit = }, {subdirs = }")

    nuke(
        exts=exts,  # type:ignore
        explicit=explicit,  # type:ignore
        subdirs=subdirs,  # type:ignore
        payload=(
            (lambda x: shutil.rmtree(x) if x.is_dir() else x.unlink())
            if args["--execute"]
            else lambda x: print(f"will remove: {x}")
        ),
    )


if __name__ == "__main__":
    main()
