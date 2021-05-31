"""LL(1) parser generator (.h header file).

This module builds the `parser.h` header file for the LL(1) table-driven parser.
"""

import datetime

from lc2_parser import parser_build
from lc2_parser import c99

HEADER_TEMPLATE = """
#ifndef PARSER_H_
#define PARSER_H_

// {name} LL(1) table-driven parser.
//
// This code was auto-generated on {timestamp}. Do not edit.

#include <map>
#include <vector>
#include <list>
#include <stack>
#include <iostream>
#include <exception>
#include <memory>

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
    NONTERMINAL_END
}} PeType;

typedef struct
{{
    PeType type;
    union {{int token; NonTerminal nt;}};
}} Pe;

struct ParseNode
{{
    NonTerminal type;
    bool empty;
    std::vector<std::unique_ptr<ParseNode>> children;
    std::vector<std::shared_ptr<Token>> terminals;
}};

class ParserError : public std::runtime_error
{{
    public:
        ParserError(const char * errmsg, int line, int position);
        int line;
        int position;
}};

// Parser class.
class Parser
{{
    private:
        std::stack<Pe> stack;
        std::stack<std::unique_ptr<ParseNode>> nodestack;

    public:
        std::unique_ptr<ParseNode> parse(std::vector<std::shared_ptr<Token>>& input);
}};
#endif
"""

class HdrParserBuilder:
    """LL(1) Parser header file.

    Includes Parser class and all type declarations."""

    # pylint: disable=too-few-public-methods

    def __init__(self, name:str, parser_builder:parser_build.ParserTable):
        self._name = name
        self._nonterminals = parser_builder.nonterminals

    def build(self) -> str:
        """Build the header file (which must be called `parser.h`)."""
        nt_definitions = [nt.enum for nt in self._nonterminals]

        # Build the header.
        return HEADER_TEMPLATE.format(
            name=self._name,
            timestamp=datetime.datetime.now().isoformat(),
            nonterminal_definitions = ",\n    ".join(nt_definitions),
        )

def main():
    """Entrypoint."""
    parser = HdrParserBuilder(
        c99.NAME,
        parser_build.ParserTable(c99.GRAMMAR, c99.START)
    )
    print(parser.build())
