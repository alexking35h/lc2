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
    int terminals;
}} Pe;

class ParserError : public std::exception
{{
    public:
        ParserError(const char * errmsg, int line, int position);
        int line;
        int position;
}};

// Parser class.
//
// Subclasses should override the production callback methods
// to handle productions derived from the grammar.
class Parser
{{
    private:
        std::stack<Pe> stack;
        std::stack<Token> tokenstack;
        void nonterminal_call(NonTerminal, int);

    public:
        void parse(std::vector<Token> input);

        // Production callback methods.
        {nonterminal_methods}
}};
#endif
"""

class HdrParserBuilder:
    """LL(1) Parser header file.

    Includes Parser class and all type declarations."""

    # pylint: disable=too-few-public-methods

    def __init__(self, name:str, parser_builder:parser_build.ParserTable):
        self._name = name
        self._productions = parser_builder.productions
        self._nonterminals = parser_builder.nonterminals

    def build(self) -> str:
        """Build the header file (which must be called `parser.h`)."""
        nt_definitions = [nt.enum for nt in self._nonterminals]

        # Build the set of production methods (this can include duplicates).
        production_methods = list()
        for production in self._productions:
            method = f'virtual void {production.method[0]}'
            method += f'({", ".join(["Token"] * production.method[1])});'
            production_methods.append(method)

        # Build the header.
        return HEADER_TEMPLATE.format(
            name=self._name,
            timestamp=datetime.datetime.now().isoformat(),
            nonterminal_definitions = ",\n    ".join(nt_definitions),
            nonterminal_methods = "\n        ".join(sorted(set(production_methods)))
        )

def main():
    """Entrypoint."""
    parser = HdrParserBuilder(
        c99.NAME,
        parser_build.ParserTable(c99.GRAMMAR)
    )
    print(parser.build())
