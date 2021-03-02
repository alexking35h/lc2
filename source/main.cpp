#include <string>
#include <iostream>
#include <sstream>

#include "parser.h"
#include "lexer.h"
#include "error.h"


int __attribute__((weak)) main()
{
    ErrorReporter er;
    while(true)
    {
        std::string st;
        std::getline(std::cin, st);
        auto tokens = Lexer(st, er).get_tokens();

        Parser p;
        p.parse(tokens);
    }
}

