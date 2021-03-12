"""LL(1) parser generator (.cpp source file).

This module builds the `parser.cpp` source file for the LL(1) table-driven parser.
"""

# pylint: disable=invalid-name, line-too-long

import datetime

from lc2_parser import parser_build
from lc2_parser import c99

IMPLEMENTATION_TEMPLATE = """
#include <map>
#include <vector>
#include <list>
#include <stack>
#include <iostream>

#include "token.h"
#include "lexer.h"
#include "error.h"
#include "parser_base.h"
#include "exception"

// {name} LL(1) table-driven parser.
//
// This code was auto-generated on {timestamp}. Do not edit.

{table}

ParserError::ParserError(const char * errmsg, int line, int position)
    : std::runtime_error(errmsg)
    , line(line)
    , position(position)
{{
}}

{nt_methods}

{nt_call_method}

{parse_method}
"""

NONTERMINAL_CALL_METHOD_TEMPLATE = """
// Call the matching non-terminal callback method for this
// production.
void ParserBase::nonterminal_call(NonTerminal nt, int terminals)
{{
    Token {token_decls};
    switch(terminals)
    {{
{token_init_switch}
    }}
    switch(nt) {{
{nt_switch}
    }}
}}
"""

PARSE_METHOD_TEMPLATE = """
void ParserBase::parse(std::vector<Token> input)
{
    int c = 0;
    auto next = input.begin(), end = input.end();

    stack.push(Pe{NONTERMINAL, .nt=NT_EXPR});

    while(!stack.empty())
    {
        Pe focus = stack.top();
        stack.pop();

        if(focus.type == NONTERMINAL)
        {
            if(table[focus.nt].find(next->get_type()) == table[focus.nt].end())
            {
                throw ParserError("Unexpected token", next->get_line(), next->get_position());
            }
            std::list<Pe> production = table[focus.nt][next->get_type()];

            production.reverse();
            for(auto pe : production)
            {
                stack.push(pe);
            }
        } else if(focus.type == TERMINAL)
        {
            if(focus.token == next->get_type())
            {
                tokenstack.push(*next);
                std::cout << "consume: " << next->get_lexeme() << std::endl;
                next++;
            } else if (focus.token == '$')
            {
                // Ignore.
            }
            else
            {
                throw ParserError("Unexpected token", next->get_line(), next->get_position());
            }
        } else if(focus.type == ACTION)
        {
            nonterminal_call(focus.nt, focus.terminals);
        }
    }
}
"""

class ImplParserBuilder:
    """LL(1) Parser C++ source file.

    Includes Parser class definition."""

    # pylint: disable=too-few-public-methods

    def __init__(self, name: str, pb:parser_build.ParserTable):
        self._name = name
        self._pb = pb

    def build(self) -> str:
        """Build the .cpp source file."""
        return IMPLEMENTATION_TEMPLATE.format(
            name=self._name,
            timestamp=datetime.datetime.now().isoformat(),
            table=self._build_table(),
            nt_methods = self._build_nonterminal_methods(),
            nt_call_method = self._build_nonterminal_switch(),
            parse_method=PARSE_METHOD_TEMPLATE
        ).rstrip()

    def _build_nonterminal_methods(self):
        """Build the default virtual production callback methods."""
        methods = {production.method for production in self._pb.productions}
        code = ""
        for method, parameters in methods:
            parameter_list = ",".join(["Token"] * parameters)
            code += f"void ParserBase::{method}({parameter_list})\n{{\n}}\n"
        return code.rstrip()

    def _build_nonterminal_switch(self):
        """Build the non-terminal callback switch statement."""
        # Tuple - non-terminal, method name, and number of arguments.
        # This may contain duplicates, so construct a set.
        productions = {(p.name, p.method[0], p.method[1]) for p in self._pb.productions}

        max_args = max(p[2] for p in productions)
        token_decls = ", ".join(f"t{j}" for j in range(max_args))

        # First switch statement, pop tokens off the stack.
        token_init_switch = ""
        for i in range(max_args, 0, -1):
            token_init_switch += "    " + f"case {i}:\n"
            token_init_switch += "        " + f"t{i-1} = tokenstack.top();\n"
            token_init_switch += "        " + "tokenstack.pop();\n"
        token_init_switch += "    case 0:\n"
        token_init_switch += "        break;"

        code = ""

        def add_line(indentation, line):
            return (" " * indentation) + line + "\n"

        # Second switch statement. Decide which production method to call.
        for nt in self._pb.nonterminals:
            code += add_line(8, f"case {nt.enum}:")
            code += add_line(12, "switch(terminals) {")

            for production in [p for p in productions if p[0] == nt]:
                code += add_line(16, f"case {production[2]}:")
                code += add_line(20, f"return {production[1]}(" + ", ".join(f"t{i}" for i in range(production[2])) + ");")

            code += add_line(12, "}")

        return NONTERMINAL_CALL_METHOD_TEMPLATE.format(
            token_decls = token_decls,
            token_init_switch = token_init_switch.rstrip(),
            nt_switch = code.rstrip()
        ).rstrip()

    def _build_table(self):
        """Build the table."""
        def element_str(element):
            if isinstance(element, parser_build.NonTerminal):
                return f"{{NONTERMINAL, .nt={element.enum}}}"
            return f"{{TERMINAL, .token={element.cdef}}}"

        def production_str(token, production):
            pstr = "{{ {}, {{ {}".format(
                token.cdef,
                ", ".join(element_str(e) for e in production.elements)
            )
            terminals = len([e for e in production.elements if isinstance(e, parser_build.Terminal) and e.name != '$'])
            pstr += f", {{ACTION, .nt={production.name.enum},"
            pstr += f" .terminals={terminals} }} }} }}"
            return pstr

        tbl = ""
        for nonterminal in self._pb.nonterminals:
            tbl += f"\n    // {nonterminal.name}\n    {{"
            for production in [p for p in self._pb.productions if p.name == nonterminal]:
                for token in production.first:
                    tbl += "\n        " + production_str(token, production) + ","
            tbl += "\n    },"
        tbl += "\n"

        return "std::map<int, std::list<Pe>> table[] = \n{" + tbl + "};"

def main():
    """Entrypoint."""
    parser = ImplParserBuilder(
        c99.NAME,
        parser_build.ParserTable(c99.GRAMMAR)
    )
    print(parser.build())
