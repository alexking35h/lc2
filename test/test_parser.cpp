#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "error.h"
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "printer.h"

TEST(ParserSuite, Primary)
{
    ErrorReporter err;
    auto tokens = Lexer("abc", err).get_tokens();
    auto as = AstBuilder().build(*Parser().parse(tokens));
    EXPECT_STREQ(PrinterVisitor().print(*as).c_str(), "(P IDENTIFIER)");

    tokens = Lexer("123", err).get_tokens();
    as = AstBuilder().build(*Parser().parse(tokens));
    EXPECT_STREQ(PrinterVisitor().print(*as).c_str(), "(P CONSTANT)");

    tokens = Lexer("\"abc\"", err).get_tokens();
    as = AstBuilder().build(*Parser().parse(tokens));
    EXPECT_STREQ(PrinterVisitor().print(*as).c_str(), "(P STRING)");

    tokens = Lexer("( 1 )", err).get_tokens();
    as = AstBuilder().build(*Parser().parse(tokens));
    EXPECT_STREQ(PrinterVisitor().print(*as).c_str(), "(P CONSTANT)");
}