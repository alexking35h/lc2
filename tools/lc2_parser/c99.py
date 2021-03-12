"""ISO C99 Grammar.

Based on: http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1256.pdf
"""

NAME = "C99"

GRAMMAR = {
    "Primary": [
        "( Expr )",
        "TOK_INTEGER_CONSTANT"
    ],
    "Expr": [
        "Primary _Expr TOK_EOF"
    ],
    "_Expr": [
        "+ Primary _Expr",
        "- Primary _Expr",
        "$"
    ]
}
