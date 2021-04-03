"""ISO C99 Grammar.

Based on: http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1256.pdf
"""

NAME = "C99"

START = "Root"

# Omissions:
# postfix-expression:
#  - ( type-name ) {initalizer-list ,? } (p69)
# unary:
#  - sizeof ( type-name ) (p90)
#  - sizeof unary-expr (p90)

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
        # TODO: 'sizeof ( type-name )'
        "Postfix",
        "TOK_PLUS_PLUS Unary",
        "TOK_MINUS_MINUS Unary",
        "& Cast",
        "* Cast",
        "+ Cast",
        "- Cast",
        "~ Cast",
        "! Cast",
    ],
    "Cast": [
        "Unary"
        # TODO: handle '( type-name ) cast-expression' (p93)
    ],
    "Multiplicative": [
        "Cast Multiplicative_End"
    ],
    "Multiplicative_End": [
        "* Cast Multiplicative_End",
        "/ Cast Multiplicative_End",
        "% Cast Multiplicative_End",
        "$"
    ],
    "Additive": [
        "Multiplicative Additive_End",
    ],
    "Additive_End": [
        "+ Multiplicative Additive_End",
        "- Multiplicative Additive_End",
        "$"
    ],
    "BitwiseShift": [
        "Additive BitwiseShift_End"
    ],
    "BitwiseShift_End": [
        "TOK_SHIFT_LEFT Additive BitwiseShift_End",
        "TOK_SHIFT_RIGHT Additive BitwiseShift_End",
        "$"
    ],
    "Expression": [
        # Temporary.
        "BitwiseShift"
    ],
    "Root": [
        "Expression TOK_EOF"
    ]
}
