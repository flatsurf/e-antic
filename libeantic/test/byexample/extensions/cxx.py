r"""
Extend byexample with natural C(++) tests

Byexample supports C++ directly through cling. However, it expects tests to be
written in cling syntax which is somewhat cryptic to readers not familiar with
cling. Also, pexpect style parsing is not terribly robust.

Here we extend byexample with a parser that can detect regular source code in
Markdown code fences and expected outputs in `// ->` comments.

We also use cppyy instead of cling which takes care of the hard part of the
parsing for us.
"""
######################################################################
#  This file is part of e-antic.
#
#        Copyright (C) 2021-2025 Julian Rüth
#
#  e-antic is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or (at your
#  option) any later version.
#
#  e-antic is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with e-antic. If not, see <https://www.gnu.org/licenses/>.
#####################################################################
from __future__ import unicode_literals
import re
from byexample.finder import ZoneDelimiter
from byexample.common import constant
import byexample.modules.cpp

stability = 'experimental'


class MarkdownCxxDelimiter(ZoneDelimiter):
    r"""
    Detects C/C++ examples in Markdown.
    """
    target = {'.md'}

    @constant
    def zone_regex(self):
        return re.compile(r'''
            # Begin with a Markdown fenced-code marker
            ^[ ]*
                (?:
                    (?P<marker>```(?:``)*)[ ]*(c|c\+\+|cpp)\b # fenced-code marker (backticks + language)
                )
            # then, grab everything until the first end marker
            (?P<zone>.*?)
            # finally, the end marker
            (?(marker)
                  ^[ ]*(?P=marker) # we must match the same amount of backticks
            )
            ''', re.DOTALL | re.MULTILINE | re.VERBOSE)

    def __repr__(self): return "```c++ ... ``` or ```cpp ... ``` or ```c ... ```"


class MarkdownHppDelimiter(ZoneDelimiter):
    r"""
    Detect C/C++ examples in Markdown comments in header files like the ones
    that we are using for standardese.
    """
    target = {'.h', '.hpp'}

    @constant
    def zone_regex(self):
        return re.compile(r'''
            # Begin with a Markdown fenced-code marker
            ^[ ]*
                (?:
                    (?P<marker>///[ ]```(?:``)*)$ # fenced-code marker (backticks)
                )
            # then, grab everything until the first end marker
            (?P<zone>.*?)
            # finally, the end marker
            (?(marker)
                  ^[ ]*(?P=marker) # we must match the same amount of backticks
            )
            ''', re.DOTALL | re.MULTILINE | re.VERBOSE)

    def __repr__(self): return "/// ``` ... ```"


class RSTCppDelimiter(ZoneDelimiter):
    r"""
    Detects C/C++ examples in reStructuredText.
    """
    target = {'.rst'}

    @constant
    def zone_regex(self):
        return re.compile(r'''
            ^\.\.[ ]code-block::[ ](c|cpp)$  # start with a .. code-block:: c(pp) marker
            (?P<zone>.*?)
            (?=^\S)  # the block ends when the indentation ends
            ''', re.DOTALL | re.MULTILINE | re.VERBOSE)

    def __repr__(self): return ".. code-block:: c or .. code-block:: cpp"


class CxxPromptFinder(byexample.modules.cpp.CppPromptFinder):
    r"""
    Detect blocks of C++ code delimited by a `// ->` comment that provides the expected output.
    """
    target = 'cpp-prompt'

    @constant
    def example_regex(self):
        return re.compile(r'''
            (?P<snippet>
                (?:^                                # match the first line
                  (?P<indent> [ ]* (?:///[ ])?)     # arbitrary indent, captured so we can strip it later
                  [^\n]+                            # followed by anything on the same line
                )
                (?:\n
                  (?P=indent) (?!//[ ]->[ ]) [^\n]* # same on the following lines until we run into an output "// -> "
                )*
            )
            \n?
            (?P<expected>
              (?:\n?
                (?P=indent)                         # the expected output starts with the indent
                (?: //[ ]->[ ] )                    # followed by the // -> marker
                [^\n]*                              # followed by the expected output on one line
              )*
            )
            \n?
            (?:[ ]* (?:///[ ]*)?\n)*                # finally, consume empty lines to prevent confusing indent detection of next block
            ''', re.MULTILINE | re.VERBOSE)

    def get_snippet_and_expected(self, match, where):
        snippet, expected = super(CxxPromptFinder, self).get_snippet_and_expected(match, where)
        expected = self._remove_expected(expected)

        return snippet, expected

    def _remove_prompts(self, snippet):
        return snippet

    def _remove_expected(self, snippet):
        marker = "// -> "
        lines = snippet.split("\n")
        lines = [line[len(marker):] if line.startswith(marker) else line for line in lines]
        return '\n'.join(lines)

    def __repr__(self): return "Unprefixed C++ Prompt Finder"


class CxxInterpreter(byexample.runner.ExampleRunner):
    r"""
    Run cppyy.cppexec on code samples.
    """
    language = 'cpp'

    def __repr__(self): return "C++ Interpreter"

    @classmethod
    def _run(cls, connection):
        import cppyy

        while True:
            try:
                source, = connection.recv()
            except EOFError:
                return

            lines = source.split('\n')
            definitions = [line for line in lines if line.startswith('#')]
            executables = [line for line in lines if not line.startswith('#') and line]

            if executables and not executables[-1].endswith(';'):
                definitions.append("#include <iostream>")
                executables[-1] = f"std::cout << std::boolalpha << ({executables[-1]});"

            exception = None

            import py.io
            capture = py.io.StdCaptureFD()

            try:
                cppyy.cppdef('\n'.join(definitions))
                cppyy.cppexec('\n'.join(executables))
            except Exception as e:
                exception = e

            stdout, stderr = capture.reset()

            connection.send((stdout, stderr, exception))

    def initialize(self, options):
        import os.path
        import sys
        sys.path.append(os.path.dirname(__file__))

        from multiprocessing import Pipe
        self._parent_connection, child_connection = Pipe()

        from multiprocessing import Process
        self._child = Process(target=CxxInterpreter._run, args=(child_connection,))
        self._child.start()

    def run(self, example, options):
        self._parent_connection.send((example.source,))
        stdout, stderr, exception = self._parent_connection.recv()
        if stderr is not None:
            import sys
            print(stderr, file=sys.stderr)
        if exception is not None:
            if stdout is not None:
                import sys
                print(stdout, file=sys.stderr)
            raise exception
        return stdout

    def shutdown(self):
        self._parent_connection.close()
        self._child.join(1)
        if self._child.exitcode is None:
            self._child.kill()
