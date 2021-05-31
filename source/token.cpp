#include <string>
#include <iostream>

#include "token.h"

Token::Token(int type, int line, int pos, const std::string& lexeme)
    : type(type)
    , line(line)
    , position(pos)
    , lexeme(lexeme)
{
}

std::ostream& operator<<(std::ostream& os, const Token& token)
{
    switch(token.type)
    {
        case TOK_IDENTIFIER:
            os << "IDENTIFIER";
            break;
        case TOK_INTEGER_CONSTANT:
            os << "CONSTANT";
            break;
        case TOK_STRING_LITERAL:
            os << "STRING";
            break;
        case TOK_EOF:
            os << "EOF";
            break;
        default:
            os << token.lexeme;
            break;
    };
    return os;
}