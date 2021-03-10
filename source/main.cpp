#include <string>
#include <iostream>
#include <sstream>

#include "parser_base.h"
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

        ParserBase p;
        try
        {
            p.parse(tokens);
        }
        catch(const ParserError& e)
        {
            std::cerr << "Error occurred during parsing: " << e.what() << std::endl;
            std::cerr << "  Line number: " << e.line << std::endl;
            std::cerr << "  Position: " << e.position << std::endl;
        }
        
    }
}

