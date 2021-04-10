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
#include <sstream>

#include "token.h"
#include "lexer.h"
#include "error.h"
#include "parser.h"
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

{parse_method}
"""

PARSE_METHOD_TEMPLATE = """
std::unique_ptr<ParseNode> Parser::parse(std::vector<std::shared_ptr<Token>>& input)
{
    int c = 0;
    auto next = input.begin(), end = input.end();

    stack.push(Pe{NONTERMINAL, .nt=%s});

    while(!stack.empty())
    {
        Pe focus = stack.top();
        stack.pop();

        if(focus.type == NONTERMINAL)
        {
            auto next_token = *next;
            if(table[focus.nt].find(next_token->get_type()) == table[focus.nt].end())
            {
                std::stringstream err;
                err << "Unexpected token:. '" << *next_token << "'";
                throw ParserError(err.str().c_str(), next_token->get_line(), next_token->get_position());
            }
            std::list<Pe> production = table[focus.nt][next_token->get_type()];

            if(production.size() == 0)
            {
                auto pn = new ParseNode{.type=focus.nt, .empty=true};    
                nodestack.top()->children.push_back(std::unique_ptr<ParseNode>(pn));
            } else {
                auto pn = new ParseNode{.type=focus.nt, .empty=false};
                nodestack.push(std::unique_ptr<ParseNode>(pn));

                production.reverse();
                for(auto pe : production)
                {
                    stack.push(pe);
                }
            }
        } else if(focus.type == TERMINAL)
        {
            auto next_token = *next;
            if(focus.token == next_token->get_type())
            {
                nodestack.top()->terminals.push_back(next_token);
                next++;
            }
            else
            {
                std::stringstream err;
                err << "Unexpected token: '" << *next_token << "'";
                throw ParserError(err.str().c_str(), next_token->get_line(), next_token->get_position());
            }
        } else if(focus.type == ACTION)
        {
            if(nodestack.top()->type == NT_ROOT)
            {
                break;
            } 
            else 
            {
                auto child = std::move(nodestack.top());
                nodestack.pop();
                nodestack.top()->children.push_back(std::move(child));
            }
        }
    }
    return std::move(nodestack.top());
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
            parse_method=PARSE_METHOD_TEMPLATE % self._pb.start.enum
        ).rstrip()

    def _build_table(self):
        """Build the table."""
        def element_str(element):
            if isinstance(element, parser_build.NonTerminal):
                return f"{{NONTERMINAL, .nt={element.enum}}}"
            return f"{{TERMINAL, .token={element.cdef}}}"

        def production_str(token, production):
            if '$' == str(production.elements[0]):
                return f"{{ {token.cdef}, {{ }} }}"

            pstr = "{{ {}, {{ {}".format(
                token.cdef,
                ", ".join(element_str(e) for e in production.elements)
            )
            terminals = len([e for e in production.elements if isinstance(e, parser_build.Terminal) and e.name != '$'])
            pstr += f", {{ACTION}} }} }}"
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
        parser_build.ParserTable(c99.GRAMMAR, c99.START)
    )
    print(parser.build())
