#include <string>
#include <iostream>

#include "token.h"

Token::Token()
    : type(TOK_NAT)
    , line(0)
    , position(0)
    , lexeme("")
{
}

Token::Token(int type, int line, int pos, std::string lexeme)
    : type(type)
    , line(line)
    , position(pos)
    , lexeme(lexeme)
{
}

std::ostream& operator<<(std::ostream& os, Token token)
{
    switch(token.get_type())
    {
        case TOK_IDENTIFIER:
            os << std::string("IDENTIFIER");
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
            os << token.get_lexeme();
            break;
    };
    return os;
}