from subprocess import PIPE, run

from docopt import docopt


def clang_format(text: str) -> str:
    result = run(["clang-format"], stdout=PIPE, input=text, encoding="ascii")
    if result.returncode != 0:
        raise Exception(str(result))
    return result.stdout


def run_docopt(doc: str | None) -> dict[str, str]:

    assert doc is not None and doc != __doc__
    return docopt(doc)
