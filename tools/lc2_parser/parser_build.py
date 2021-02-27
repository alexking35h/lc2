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

Grammar = {
    "Primary": [
        "( Expr )",
        "CONTSTANT"
    ],
    "Expr": [
        "Primary _Expr"
    ],
    "_Expr": [
        "+ Primary _Expr",
        "- Primary _Expr",
        "$"
    ]
}

class NonTerminal:
    def __init__(self):
        self.first = set()
        self.follow = set()

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
    return re.match('^([A-Z][a-z]+)+', name) is not None

class ParserBuilder:

    def __init__(self, grammar):
        self._nonterminals = {n:NonTerminal() for n in grammar.keys()}
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

HEADER_TEMPLATE = """
"""

class HeaderParserBuilder:

    def __init__(self, pb):
        self._pb = pb
    
    def build(self):


IMPLEMENTATION_TEMPLATE = """
typedef enum
{{
    {nonterminal_definitions}
}};
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
}};
{table}
"""

class ImplParserBuilder:

    def __init__(self, pb):
        self._pb = pb
    
    def build(self):
        return IMPLEMENTATION_TEMPLATE.format(
            nonterminal_definitions="lk",
            table=self._build_table()
        )
    
    def _build_table(self):
        def token_str(t):
            if re.match('^[A-Z]+$', t):
                return t
            return f"'{t}'"

        def element_str(e):
            if is_nonterminal(e):
                return f"{{NONTERMINAL, .nt={e}}}"
            if re.match('^[A-Z]+$', e):
                return f"{{TERMINAL, .token={e}}}"
            return f"{{TERMINAL, .token='{e}'}}"

        def production_str(token, production):
            return "{{ {}, {{ {} }} }}".format(
                token_str(token),
                ", ".join(element_str(e) for e in production.elements)
            )
        tbl = ""
        for nonterminal in self._pb.nonterminals:
            tbl += f"\n    // {nonterminal}\n    {{"
            for production in [p for p in self._pb.productions if p.name == nonterminal]:
                for token in production.first:
                    tbl += "\n        " + production_str(token, production)
            tbl += "\n    }"
        tbl += "\n"

        return "str::map<int, std::list<Pe>> table[] = \n{" + tbl + "};"

builder = ImplParserBuilder(ParserBuilder(Grammar)).build()
print(builder)

    # p = ParserGenerator(Grammar)
    # for n, nt in ParserGenerator(Grammar)._nonterminals.items():
    #     print(f"{n}: {str(nt.first)}")
    #     print(f"{n}: {str(nt.follow)}")

    # print("-----")
    # for prod in p._productions:
    #     print(f"{prod.name}: {str(prod.first)}")

    # for n in p._table:
    #     print(f"{n}:")
    #     for t in p._table[n]:
    #         print(f"  {t}: {str(p._table[n][t])}")
