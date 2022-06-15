#!/usr/bin/env python3
"""
vec_of_any.py:
    creates boilerplate for variable argument function "vec_of_any()".
    vec_of_any generates anonymous string vector, with maximum <size> of
    different types of arguments, at the cost of being even more verbose.
    example usage - "vec_of_any(3, "b", 2.0f)

usage:
    vec_of_any.py [--help] [options] (--dry | <path>)

options:
    -h, --help            show this help message and exit
    -n <N>, --size <N>    number of maximum argument size. [default: 10]
    -d, --dry             write to stdout instead of file.
"""

import shutil
from dataclasses import dataclass
from functools import cached_property
from pathlib import Path
from textwrap import dedent

from cpputil import clang_format
from create import wrap_header

template = dedent(
    """\
    // do not try to directly edit this file.
    // generate using vec_of_any.py instead
    #include <sstream>

    #define VA(param) util::vec_of_any param

    namespace util {{

    using std::string;
    using std::vector;
    using std::stringstream;

    template <typename T>
    static string s(T t) {{
        stringstream ss;
        ss << t;
        return ss.str();
    }}

    {text}
    }} // namespace util
    """
)


@dataclass
class Template:
    size: int = 10

    def maybe(self, then: str) -> str:
        return "" if self.size == 0 else then

    def create_template_args(self) -> str:
        args = ", ".join(f"class T{n}" for n in range(self.size))
        return self.maybe(f"template <{args}>")

    def create_args(self, *, with_type: bool = False) -> str:
        arg = "T{n} arg{n}" if with_type else "s(arg{n})"
        return self.maybe(
            ", ".join([arg.format(n=n) for n in range(self.size)])
        )

    def create_args_array(self) -> str:
        return self.maybe(
            f"const string args[{self.size}] = {{{self.create_args()}}};"
        )

    def create_return(self) -> str:
        return f"""return vector<string>({self.maybe(f"args, args + {self.size}")});"""

    def __str__(self) -> str:
        return dedent(
            f"""\
            {self.create_template_args()}
            inline vector<string> vec_of_any({self.create_args(with_type=True)}) {{
                {self.create_args_array()}
                {self.create_return()}
            }}
            """
        )


@dataclass(frozen=True)
class VecOfAny:
    size: int = 10

    @cached_property
    def as_text(self) -> str:
        text = "\n".join([str(Template(n)) for n in range(self.size + 1)])
        return template.format(text=text)

    def __repr__(self) -> str:
        return self.as_text


def create_formatted_result(size: int, path: Path | None) -> str:
    text = VecOfAny(size).as_text
    if path:
        text = wrap_header(text, path)
    if shutil.which("clang-format"):
        text = clang_format(text)

    return text + "\n"


def main():
    from docopt import docopt

    assert __doc__ is not None
    args: dict[str, str] = docopt(__doc__)

    # print(args)

    size = int(args["--size"])
    path = Path(args["<path>"]) if args["<path>"] else None
    text = create_formatted_result(size, path)

    if args["--dry"]:
        print(text)
    elif path is not None:
        path.write_text(text)


if __name__ == "__main__":
    main()
