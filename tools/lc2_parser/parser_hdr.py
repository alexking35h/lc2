import re

from . import parser_build
from . import grammar

HEADER_TEMPLATE = """
#include <map>
#include <vector>
#include <list>
#include <stack>
#include <iostream>

#include "token.h"
#include "lexer.h"
#include "error.h"

typedef enum
{{
    {nonterminal_definitions}
}} NonTerminal;
typedef enum
{{
    TERMINAL,
    NONTERMINAL,
    ACTION
}} PeType;
typedef struct
{{
    PeType type;
    union {{int token; NonTerminal nt;}};
}} Pe;
class Parser
{{
    private:
        std::stack<Pe> stack;
        void nonterminal_call(NonTerminal);

    public:
        void parse(std::vector<Token> input);
        {nonterminal_methods}
}};
"""

class HdrParserBuilder:

    def __init__(self, pb):
        self._nonterminals = pb.nonterminals.values()
    
    def build(self):
        nt_definitions = [nt.enum for nt in self._nonterminals]
        nt_methods = [f'virtual void {nt.method}();' for nt in self._nonterminals]
        return HEADER_TEMPLATE.format(
            nonterminal_definitions = ",\n    ".join(nt_definitions),
            nonterminal_methods = "\n        ".join(nt_methods)
        )

def main():
    parser = HdrParserBuilder(
        parser_build.ParserBuilder(grammar.Grammar)
    )
    print(parser.build())
