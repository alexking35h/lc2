#include <string>

#include "token.h"

Token::Token(TokenType type, int line, int pos, std::string lexeme)
    : type(type)
    , line(line)
    , position(pos)
    , lexeme(lexeme)
{
}