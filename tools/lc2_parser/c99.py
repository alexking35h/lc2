"""ISO C99 Grammar.

http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1256.pdf
"""

NAME = "C99"

START = "Root"

# Omissions:
# postfix-expression:
#  - ( type-name ) {initalizer-list ,? } (p69)
# unary:
#  - sizeof ( type-name ) (p90)
#  - sizeof unary-expr (p90)
# expressions:
#  - expression , assignment-expression (p106)

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
        "Assignment ArgumentExpressionList_End",
        "$"
    ],
    "ArgumentExpressionList_End": [
        ", Assignment ArgumentExpressionList_End",
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
    "Relational": [
        "BitwiseShift Relational_End"
    ],
    "Relational_End": [
        "< BitwiseShift Relational_End",
        "> BitwiseShift Relational_End",
        "TOK_GE BitwiseShift Relational_End",
        "TOK_LE BitwiseShift Relational_End",
        "$"
    ],
    "Equality": [
        "Relational Equality_End",
    ],
    "Equality_End": [
        "TOK_EQ Relational Equality_End",
        "TOK_NE Relational Equality_End",
        "$"
    ],
    "BitwiseAnd": [
        "Equality BitwiseAnd_End"
    ],
    "BitwiseAnd_End": [
        "& Equality BitwiseAnd_End",
        "$"
    ],
    "BitwiseExclusiveOr": [
        "BitwiseAnd BitwiseExclusiveOr_End"
    ],
    "BitwiseExclusiveOr_End": [
        "^ BitwiseAnd BitwiseExclusiveOr_End",
        "$"
    ],
    "BitwiseInclusiveOr": [
        "BitwiseExclusiveOr BitwiseInclusiveOr_End"
    ],
    "BitwiseInclusiveOr_End": [
        "| BitwiseExclusiveOr BitwiseInclusiveOr_End",
        "$"
    ],
    "LogicalAnd": [
        "BitwiseInclusiveOr LogicalAnd_End"
    ],
    "LogicalAnd_End": [
        "TOK_AND_OP BitwiseInclusiveOr LogicalAnd_End",
        "$"
    ],
    "LogicalOr": [
        "LogicalAnd LogicalOr_End"
    ],
    "LogicalOr_End": [
        "TOK_OR_OP LogicalAnd LogicalOr_End",
        "$"
    ],
    "Conditional": [
        "LogicalOr Conditional_End"
    ],
    "Conditional_End": [
        "? Expression : Conditional",
        "$"
    ],
    "Assignment": [
        "Conditional Assignment_End"
    ],
    "Assignment_End": [
        "= Assignment",
        "TOK_MUL_ASSIGN Assignment",
        "TOK_DIV_ASSIGN Assignment",
        "TOK_MOD_ASSIGN Assignment",
        "TOK_PLUS_ASSIGN Assignment",
        "TOK_MINUS_ASSIGN Assignment",
        "TOK_SHIFT_LEFT_ASSIGN Assignment",
        "TOK_SHIFT_RIGHT_ASSIGN Assignment",
        "TOK_AND_ASSIGN Assignment",
        "TOK_XOR_ASSIGN Assignment",
        "TOK_OR_ASSIGN Assignment",
        "$"
    ],
    "Expression": [
        
        "Assignment"
    ],
    "Declaration": [
        "DeclarationSpecifiers InitDeclaratorList",
    ],
    "DeclarationSpecifiers": [
        "TOK_STATIC DeclarationSpecifiers",
        "TOK_REGISTER DeclarationSpecifiers",
        "TOK_VOID DeclarationSpecifiers",
        "TOK_CHAR DeclarationSpecifiers",
        "TOK_SHORT DeclarationSpecifiers",
        "TOK_INT DeclarationSpecifiers",
        "TOK_SIGNED DeclarationSpecifiers",
        "TOK_UNSIGNED DeclarationSpecifiers",
        "TOK_CONST DeclarationSpecifiers",
        "$"
    ],
    "InitDeclaratorList": [
        "InitDeclarator InitDeclaratorList_End"
    ],
    "InitDeclaratorList_End": [
        ", InitDeclarator InitDeclaratorList_End",
        "$"
    ],
    "InitDeclarator": [
        "Declarator InitDeclarator_End"
    ],
    "InitDeclarator_End": [
        # Todo. Should be '= Initializer (p109)'
        "= Expression",
        "$"
    ],
    "Declarator": [
        #TODO P126
        "TOK_IDENTIFIER"
    ],
    "Root": [
        "Expression TOK_EOF",
        "Declaration TOK_EOF"
    ]
}
