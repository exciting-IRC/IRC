#!/usr/bin/env python3
"""
container_of.py:
    creates boilerplate for variable argument function "container_of()".
    container_of generates anonymous container, with maximum <size> of arguments.
    default writes stdout, provide <path> to write to file.
    example usage - "container_of<vector<string> >("a", "b", "c)"

usage:
    container_of.py [--help] [options] [ <path> ]

options:
    -h, --help            show this help message and exit
    -n <N>, --size <N>    number of maximum argument size. [default: 10]
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
    // generate using container_of.py instead

    #define MAP_OF(K, V, param) container_of<std::map<K, V> > param
    #define VEC_OF(T, param) container_of<std::vector<T> > param
    #define V(param) VEC_OF(std::string, param)
    #define T typename C::value_type

    namespace util {{
    template <typename K, typename V>
    std::pair<K, V> p(K k, V v) {{
      return std::make_pair(k, v);
    }}
    }} // namespace util

    // hardcoded size 0 case
    template <typename C>
    inline C container_of() {{
      return C();
    }}
    // end of hardcoded size 0 cases

    {text}

    #undef T
    """
)


@dataclass
class Template:
    """
    size must be greater than 0

    typename C <- container type
    typename T <- value type, expands to typename C::value_type
    """

    size: int = 10

    def create_args(self, *, with_type: bool = False) -> str:
        arg = "T arg{n}" if with_type else "arg{n}"
        return ", ".join(arg.format(n=n) for n in range(self.size))

    def create_args_array(self) -> str:
        return f"const T args[{self.size}] = {{{self.create_args()}}};"

    def create_return(self) -> str:
        return f"""return C({f"args, args + {self.size}"});"""

    def __str__(self) -> str:
        return dedent(
            f"""\
            template <typename C>
            inline C container_of({self.create_args(with_type=True)}) {{
                {self.create_args_array()}
                {self.create_return()}
            }}
            """
        )


@dataclass
class ContainerOfArgs:
    size: int = 10

    @cached_property
    def as_text(self) -> str:
        text = "\n".join(str(Template(n)) for n in range(1, self.size + 1))
        return template.format(
            text=text,
        )

    def __repr__(self) -> str:
        return self.as_text


def create_result(size: int, path: Path | None) -> str:
    text = ContainerOfArgs(size).as_text
    if path:
        text = wrap_header(text, path)
    if size < 100 and shutil.which("clang-format"):
        text = clang_format(text)
    else:
        text = f"// clang-format: off\n{text}"

    return text + "\n"


def main():
    from docopt import docopt

    assert __doc__ is not None
    args: dict[str, str] = docopt(__doc__)

    # print(args)

    size = int(args["--size"])
    path = Path(args["<path>"]) if args["<path>"] else None
    text = create_result(size, path)

    if path:
        path.write_text(text)
    else:
        print(text)


if __name__ == "__main__":
    main()
