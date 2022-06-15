#!/usr/bin/env python3
"""
create.py: create boilerplate for *.cpp, *.hpp, *.tpp

usage:
    create.py [--help] [options] [-n | -N | --namespace <N> ] <path>

options:
    -h, --help               show this help message and exit
    -n                       use parent directory name as namespace
    -N                       use nested directory names as namespace
    --namespace <N>          choose namespace to use.
    -s, --switch             open created file on vscode.
    -f, --force              overwrite existing files.
    -r <R>, --root <R>       root of directory path [default: src]
"""
from pathlib import Path

from cpputil import run_docopt


def wrap_with(body: str, wrap: tuple[str, str], *, distance: int = 1) -> str:
    nl = distance * "\n"
    return f"{wrap[0]}{nl}{body}{nl}{wrap[1]}"


def wrap_header(body: str, path: Path) -> str:
    guard = "_".join(path.parts).replace(".", "_").upper()
    header = (
        f"#ifndef {guard}\n" f"#define {guard}",
        f"#endif // {guard}",
    )
    return wrap_with(body, header, distance=2)


def base_category_name(stem: str) -> str:
    return stem.rsplit("_", maxsplit=1)[0]


def get_include(path: Path) -> str:
    match path.suffix:
        case ".hpp":
            return ""
        case ".tpp":
            name = base_category_name(path.stem)
        case ".cpp":
            name = path.parent.stem.removeprefix("lib")
        case _:
            raise ValueError(f"Unknown file type: {path}")

    return f"#include <{'/'.join(path.parent.parts)}/{name}.hpp>"


def get_nested_namespace(names: tuple[str]) -> str:
    names = tuple(n.removeprefix("lib") for n in names)
    begins = "\n".join([f"namespace {ns} {{" for ns in names])
    ends = "\n".join([f"}}  // namespace {ns}" for ns in reversed(names)])

    return f"{begins}\n{ends}"


def save_text_to(text: str, path: Path) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(f"{text}\n")
    print(text)


def create_text(path: Path, *, namespace: tuple[str] | None) -> str:
    text = get_include(path)
    if namespace:
        text += f"\n\n{get_nested_namespace(namespace)}"

    return text if path.suffix == ".cpp" else wrap_header(text, path)


def get_namespace(path: Path, args: dict[str, str]) -> tuple[str] | None:
    if namespace := args["--namespace"]:
        return (namespace,)
    elif args["-n"]:
        return (path.parent.stem,)
    elif args["-N"]:
        return tuple(path.parent.parts)


def main():
    args = run_docopt(__doc__)

    # print(args)

    path = Path(args["<path>"])
    fullpath = Path(__file__).parent / args["--root"] / args["<path>"]

    if not(args['--force']) and fullpath.exists():
        print("file already exists")
        return

    namespace = get_namespace(path, args)
    text = create_text(path, namespace=namespace)
    save_text_to(text, fullpath)

    if args["--switch"]:
        from subprocess import run

        run(["code", str(fullpath)])


if __name__ == "__main__":
    main()
