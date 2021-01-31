#include <istream>

#include "lexer.h"

Lexer::Lexer(std::istream input)
    : input(input)
{
}

std::vector<pToken> Lexer::get_tokens()
{
    return tokens;
}