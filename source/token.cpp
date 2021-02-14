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