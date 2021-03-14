#include <string>
#include <iostream>
#include <sstream>
#include <memory>

#include "parser.h"
#include "lexer.h"
#include "error.h"
#include "ast.h"
#include "token.h"
#include "parser.h"
#include "printer.h"


int __attribute__((weak)) main()
{
    ErrorReporter er;

    while(true)
    {
        std::string input;
        std::getline(std::cin, input);
        auto tokens = Lexer(input, er).get_tokens();

        Parser parser;
        PrinterVisitor printer;

        try
        {
            auto parse_root = parser.parse(tokens);
            auto ast = AstBuilder().build(*parse_root);
            std::cout << printer.print(*ast) << std::endl;
        }
        catch(const ParserError& e)
        {
            std::cerr << "Error occurred during parsing: " << e.what() << std::endl;
            std::cerr << "  Line number: " << e.line << std::endl;
            std::cerr << "  Position: " << e.position << std::endl;
        }
        
    }
}

