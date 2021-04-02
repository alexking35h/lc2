"""ISO C99 Grammar.

Based on: http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1256.pdf
"""

NAME = "C99"

START = "Root"

# Omissions:
# postfix-expression:
#  - ( type-name ) {initalizer-list ,? } (p69)

GRAMMAR = {
    "Primary": [
        "TOK_IDENTIFIER",
        "TOK_INTEGER_CONSTANT",
        "TOK_STRING_LITERAL",
        "( Expression )"
    ],
    "Postfix": [
        "Primary Postfix_End",
    ],
    "Postfix_End": [
        "[ Expression ] Postfix_End",
        "( ArgumentExpressionList ) Postfix_End",
        ". TOK_IDENTIFIER Postfix_End",
        "TOK_POINTER_OP TOK_IDENTIFIER Postfix_End",
        "TOK_PLUS_PLUS Postfix_End",
        "TOK_MINUS_MINUS Postfix_End",
        "$"
    ],
    "ArgumentExpressionList": [
        # TODO: Should be assignment expression (p70)
        "Expression ArgumentExpressionList_End",
        "$"
    ],
    "ArgumentExpressionList_End": [
        ", Expression ArgumentExpressionList_End",
        "$"
    ],
    "Unary": [
        "Postfix",
        "TOK_PLUS_PLUS Unary",
        "TOK_MINUS_MINUS Unary",
        "& Cast",
        "* Cast",
        "+ Cast",
        "- Cast",
        "~ Cast",
        "! Cast"
    ],
    "Cast": [
        "Unary"
        # TODO: handle '( type-name ) cast-expression' (p93)
    ],
    "Expression": [
        # Temporary.
        "Unary"
    ],
    "Root": [
        "Expression TOK_EOF"
    ]
}
