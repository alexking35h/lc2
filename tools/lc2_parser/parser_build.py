"""LL(1) Parser Generator.

Generate a C++ table-driven LL1(1) parser from a compatible grammar.
The grammar is expressed as a set of productions in a dictionary.
The `ParserBuilder` class reads this grammar, and generates the parser lookup table.
Other modules should implement C++ header/implementation generation using this class.

The grammar is a Python dictionary. Keys are nonterminals, values are lists of productions.
Each production is a string. Terminals and non-terminals should follow this format:
* Terminals:
  * `ALL_CAPS`
  * `+`, `-` - single-characters
* Non-terminals:
  * `SnakeCase`
* Special characters:
  * `$` - Empty production.
"""

# pylint: disable=invalid-name

import re

class NonTerminal:
    """Non-Terminal abstraction."""

    def __init__(self, name:str):
        self.name = name
        self.first = set()
        self.follow = set()

    @property
    def enum(self) -> str:
        """C++ NonTerminal definition for this nonterminal."""
        return f'NT_{self.name.upper()}'

    def __str__(self) -> str:
        return self.name

class Terminal:
    """Terminal abstraction."""
    def __init__(self, token:str):
        self.name = token

    @property
    def cdef(self) -> str:
        """C++ representation for this Terminal.

        This is either a character (e.g., ';'), or a macro definition (e.g., EXPR).
        """
        return self.name if re.match('^[A-Z_]+$', self.name) else f"'{self.name}'"

    def __str__(self):
        return self.name

class Production:
    """Grammar production abstraction.

    Productions are defined by a non-terminal (head) - the symbol that derives this production,
    and a list of terminals/non-terminals in this production (elements).
    """
    def __init__(self, head_str:str, rule:str, nonterminals:list[NonTerminal]):
        """
        Initialize Production instance from a rule.

        `nonterminals` is a dictionary containing all NonTerminal symbols used by the grammar. This should
        include all non-terminals in `rule` and `head`.
        """
        self.head = nonterminals[head_str]
        self.elements = []
        self.first = set()

        for element in rule.split(' '):
            if re.match('^(_?[A-Z][a-z]+)+', element):
                self.elements.append(nonterminals[element])
            else:
                self.elements.append(Terminal(element))

    def __str__(self) -> str:
        return f"{self.name}: {' '.join([str(e) for e in self.elements])}"

class ParserTable:
    """LL(1) Table-driven parser

    This class generates the LL(1) parser table for the given grammar.
    """

    def __init__(self, grammar:dict[str,list[str]], start:str):
        self._nonterminals = {n:NonTerminal(n) for n in grammar.keys()}
        self._productions = [Production(name, rule, self._nonterminals)
                             for name in grammar for rule in grammar[name]]
        self._table = dict()
        self._start = self._nonterminals[start]

        self._build_first_sets()
        self._build_follow_sets()
        self._build_augmented_first_sets()
        self._build_table()

    @property
    def nonterminals(self) -> list[NonTerminal]:
        """List of non-terminals in the table/grammar."""
        return self._nonterminals.values()

    @property
    def productions(self) -> list[Production]:
        """List of productions in the table/grammar."""
        return self._productions
    
    @property
    def start(self) -> NonTerminal:
        """Get start symbol."""
        return self._start

    @property
    def table(self) -> dict[NonTerminal, dict[Terminal, Production]]:
        """LL(1) parser table.

        The table is a dictionary mapping non-terminal names to another
        dictionary, mapping terminal symbols (the lookahead) to productions.
        """
        return self._table

    def _build_first_sets(self):
        prior = list()

        while prior != [nt.first for nt in self._nonterminals.values()]:
            prior = [nt.first for nt in self._nonterminals.values()]

            for p in self._productions:
                for element in p.elements:
                    if element == p:
                        raise Exception("Left recursion.")
                    
                    if isinstance(element, NonTerminal):
                        first = element.first
                    elif isinstance(element, Terminal):
                        first = {element}
                    
                    p.head.first = p.head.first.union(first)

                    if '$' not in [str(p) for p in first]:
                        break

    def _build_follow_sets(self):
        prior = list()

        while prior != [nt.follow for nt in self._nonterminals.values()]:
            prior = [nt.follow for nt in self._nonterminals.values()]

            for production in self._productions:
                trailer = production.head.follow

                for element in production.elements[::-1]:
                    if isinstance(element, Terminal):
                        trailer = {element}
                        continue

                    nt = element
                    nt.follow = nt.follow.union(trailer)

                    if '$' in [str(t) for t in nt.first]:
                        trailer = {t for t in nt.first if str(t) != '$'}.union(nt.follow)
                    else:
                        trailer = nt.first

    def _build_augmented_first_sets(self):
        for production in self._productions:
            if isinstance(production.elements[0], NonTerminal):
                production.first = production.first.union(production.elements[0].first)
            else:
                production.first.add(production.elements[0])

            if '$' in [str(t) for t in production.first]:
                production.first = {e for e in production.first if e.name != '$'}
                production.first = production.first.union(production.head.follow)

    def _build_table(self):
        self._table = {p.head:dict() for p in self._productions}
        for production in self._productions:
            for terminal in production.first:
                self._table[production.head][terminal] = production
