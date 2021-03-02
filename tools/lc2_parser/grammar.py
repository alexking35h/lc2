Grammar = {
    "Primary": [
        "( Expr )",
        "TOK_INTEGER_CONSTANT"
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