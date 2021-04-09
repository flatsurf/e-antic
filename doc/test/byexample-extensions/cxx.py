r"""
Extend byexample with natural C(++) tests

Byexample supports C++ directly through cling. However, it expects tests to be
written cling syntax which is somewhat cryptic to readers not familiar with
cling.

Here we extend byexample with a parser that can detect regular source code in
MarkDown code fences and expected outputs in `// ->` comments.

We also extend the cling invocation to pass more parameters to cling through
the `EXTRA_CLING_ARGS` environment variable.
"""
######################################################################
#  This file is part of e-antic.
#
#        Copyright (C) 2021 Julian Rüth
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
    Detects C/C++ examples in MarkDown.
    """
    target = {'.md'}

    @constant
    def zone_regex(self):
        return re.compile(r'''
            # Begin with a markdown fenced-code marker
            ^[ ]*
                (?:
                    (?P<marker>```(?:``)*)[ ]*(c|c\+\+|cpp)?  # fenced-code marker (backticks + language)
                )
            # then, grab everything until the first end marker
            (?P<zone>.*?)
            # finally, the end marker
            (?(marker)    # if we matched a fenced-code maker previously
                  ^[ ]*(?P=marker) # then we must match the same amount of backticks
            )
            ''', re.DOTALL | re.MULTILINE | re.VERBOSE)

    def __repr__(self): return "```c++ ... ``` or ```cpp ... ``` or ```c ... ```"

class MarkdownHppDelimiter(ZoneDelimiter):
    r"""
    Detect C/C++ examples in MarkDown comments in Header files like the ones
    that we are using for standardese.
    """
    target = {'.h', '.hpp'}

    @constant
    def zone_regex(self):
        return re.compile(r'''
            # Begin with a markdown fenced-code marker
            ^[ ]*
                (?:
                    (?P<marker>///[ ]```(?:``)*)[ ]*(c|c\+\+|cpp)  # fenced-code marker (backticks + language)
                )
            # then, grab everything until the first end marker
            (?P<zone>.*?)
            # finally, the end marker
            (?(marker)    # if we matched a fenced-code maker previously
                  ^[ ]*(?P=marker) # then we must match the same amount of backticks
            )
            ''', re.DOTALL | re.MULTILINE | re.VERBOSE)

    def __repr__(self): return "/// ```c++ ... ``` or /// ```cpp ... ``` or /// ```c ... ```"

class CxxPromptFinder(byexample.modules.cpp.CppPromptFinder):
    r"""
    Detects blocks of C++ code delimited ended by a `// ->` comment that provides the expected output.
    """
    target = 'cpp-prompt'

    @constant
    def example_regex(self):
        return re.compile(r'''
            (?P<snippet>
                (?:^                                # match the first line
                  (?P<indent> [ ]* (?:///[ ])?)     # an arbitrary amount of indent (we capture it so we can strip it later)
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
            (?:[ ]* (?:///[ ]*)?\n)*                # Finally, consume empty lines so that they do not confuse the indent detection of the next block
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
        lines = [ line[len(marker):] if line.startswith(marker) else line for line in lines]
        return '\n'.join(lines)

    def __repr__(self): return "Unprefixed C++ Prompt Finder"

class CxxInterpreter(byexample.modules.cpp.CPPInterpreter):
    r"""
    Run cling on code samples with `EXTRA_CLING_ARGS`.
    """
    language = 'cpp'

    def get_default_cmd(self, *args, **kwargs):
        ret = super(CxxInterpreter, self).get_default_cmd(*args, **kwargs)
        import os
        ret[1]['a'].extend(os.environ.get('EXTRA_CLING_ARGS', '').split())
        return ret

    def __repr__(self): return "C++ Interpreter"
