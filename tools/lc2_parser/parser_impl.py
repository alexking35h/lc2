
import re

from . import parser_build
from . import grammar

IMPLEMENTATION_TEMPLATE = """
#include <map>
#include <vector>
#include <list>
#include <stack>
#include <iostream>

#include "token.h"
#include "lexer.h"
#include "error.h"
#include "parser.h"

{table}

{nt_methods}

void Parser::nonterminal_call(NonTerminal nt)
{{
{nt_switch}
}}
"""

IMPLEMENTATION = """
void Parser::parse(std::vector<Token> input)
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
                std::cout << "consume: " << next->get_lexeme() << std::endl;
                next++;
            } else if (focus.token == '$')
            {
                next++;
            }
            else
            {
                std::cout << "unexpected: " << next->get_lexeme() << std::endl;
            }
        } else if(focus.type == ACTION)
        {
            nonterminal_call(focus.nt);
        }
    }
}
"""

class ImplParserBuilder:

    def __init__(self, pb):
        self._pb = pb
        self._nonterminals = {nt:f'NT_{nt.upper()}' for nt in pb.nonterminals}
    
    def build(self):
        return IMPLEMENTATION_TEMPLATE.format(
            table=self._build_table(),
            nt_methods = self._build_nonterminal_methods(),
            nt_switch = self._build_nonterminal_switch()
        ) + IMPLEMENTATION
    
    def _build_nonterminal_methods(self):
        code = ""
        for nt in self._nonterminals:
            method = "void Parser::{}()".format(re.sub('([a-z])([A-Z])', r'\1_\2', nt).lower())
            code += method + "\n{\n}\n"
        return code
    
    def _build_nonterminal_switch(self):
        code = "    switch(nt)\n"
        code += "    {\n"
        for name, nt in self._nonterminals.items():
            code += f"        case {nt}:\n"
            code += "            return {}();\n".format(re.sub('([a-z])([A-Z])', r'\1_\2', name).lower())
        code += "    }"
        return code
    
    def _build_table(self):
        def token_str(t):
            if re.match('^[A-Z_]+$', t):
                return t
            return f"'{t}'"

        def element_str(e):
            if parser_build.is_nonterminal(e):
                return f"{{NONTERMINAL, .nt={self._nonterminals[e]}}}"
            if len(e) == 1:
                return f"{{TERMINAL, .token='{e}'}}"
            return f"{{TERMINAL, .token={e}}}"

        def production_str(token, production):
            s = "{{ {}, {{ {}".format(
                token_str(token),
                ", ".join(element_str(e) for e in production.elements)
            )
            return s + f", {{ACTION, .nt={self._nonterminals[production.name]} }} }} }}"

        tbl = ""
        for nonterminal in self._pb.nonterminals:
            tbl += f"\n    // {nonterminal}\n    {{"
            for production in [p for p in self._pb.productions if p.name == nonterminal]:
                for token in production.first:
                    tbl += "\n        " + production_str(token, production) + ","
            tbl += "\n    },"
        tbl += "\n"

        return "std::map<int, std::list<Pe>> table[] = \n{" + tbl + "};"

def main():
    parser = ImplParserBuilder(
        parser_build.ParserBuilder(grammar.Grammar)
    )
    print(parser.build())