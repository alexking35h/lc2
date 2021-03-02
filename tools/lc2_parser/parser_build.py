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

import re

class NonTerminal:
    def __init__(self, name):
        self.name = name
        self.first = set()
        self.follow = set()
    
    @property
    def enum(self):
        return f'NT_{self.name}'
    
    @property
    def method(self):
        return re.sub('([a-z])([A-Z])', r'\1_\2', self.name).lower()

class Production:
    def __init__(self, name, rule):
        self.name = name
        self.elements = rule.split(' ')
        self.first = set()
    
    def __str__(self):
        return f"{self.name}: {' '.join(self.elements)}"

def is_terminal(name):
    if re.match('^[A-Z_]+$', name):
        return True
    if re.match('^[^A-Za-z_]$', name):
        return True
    return False

def is_nonterminal(name):
    return re.match('^(_?[A-Z][a-z]+)+', name) is not None

class ParserBuilder:

    def __init__(self, grammar):
        self._nonterminals = {n:NonTerminal(n) for n in grammar.keys()}
        self._productions = [Production(name, rule) for name in grammar for rule in grammar[name]]
        self._table = dict()

        self._build_first_sets()
        self._build_follow_sets()
        self._build_augmented_first_sets()
        self._build_table()
    
    @property
    def nonterminals(self):
        return self._nonterminals
    
    @property
    def productions(self):
        return self._productions
    
    @property
    def table(self):
        return self._table

    def _build_first_sets(self):
        prior = list()

        while prior != [nt.first for nt in self._nonterminals.values()]:
            prior = [nt.first for nt in self._nonterminals.values()]

            for p in self._productions:
                for element in p.elements:
                    if element == p.name:
                        raise Exception("Left recursion.")

                    if is_terminal(element):
                        self._nonterminals[p.name].first.add(element)
                        break

                    if is_nonterminal(element):
                        first = self._nonterminals[element].first
                        self._nonterminals[p.name].first = self._nonterminals[p.name].first.union(first)

                        if '$' not in first:
                            break
    
    def _build_follow_sets(self):
        prior = list()
        
        while prior != [nt.follow for nt in self._nonterminals.values()]:
            prior = [nt.follow for nt in self._nonterminals.values()]

            for production in self._productions:
                nonterminal = self._nonterminals[production.name]
                trailer = nonterminal.follow

                for element in production.elements[::-1]:
                    if is_terminal(element):
                        trailer = set(element)
                        continue

                    nt = self._nonterminals[element]
                    nt.follow = nt.follow.union(trailer)

                    if '$' in nt.first:
                        trailer = (nt.first - set('$')).union(nt.follow)
                    else:
                        trailer = nt.first

    def _build_augmented_first_sets(self):
        for production in self._productions:
            if is_terminal(production.elements[0]):
                production.first.add(production.elements[0])
            else:
                production.first = self._nonterminals[production.elements[0]].first
            
            if '$' in production.first:
                production.first = production.first.union(self._nonterminals[production.name].follow)
    
    def _build_table(self):
        self._table = {p.name:dict() for p in self._productions}
        for production in self._productions:
            for terminal in production.first:
                self._table[production.name][terminal] = production