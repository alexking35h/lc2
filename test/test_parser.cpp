#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "error.h"
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "printer.h"

void expect_ast(const char * src, const char * expected_ast)
{
    ErrorReporter err;
    std::shared_ptr<AstNode> ast_root;
    std::vector<std::shared_ptr<Token>> tokens = Lexer(src, err).get_tokens();
    std::unique_ptr<ParseNode> parse_tree = std::move(Parser().parse(tokens));
    ast_root = AstBuilder().build(*parse_tree);
    std::string ast_str = PrinterVisitor().print(*ast_root);
    EXPECT_STREQ(ast_str.c_str(), const_cast<char *>(expected_ast));
}

TEST(ParserSuite, Primary)
{
    expect_ast("abc", "(P IDENTIFIER)");
    expect_ast("123", "(P CONSTANT)");
    expect_ast("\"abc\"", "(P STRING)");
    expect_ast("( 1 )", "(P CONSTANT)");
}

TEST(ParserSuite, Postfix)
{
    expect_ast("1 ++", "(PF ++, (P CONSTANT))");
    expect_ast("1 --", "(PF --, (P CONSTANT))");
    expect_ast("1 [ A ]", "(PF [], (P CONSTANT), (P IDENTIFIER))");
    expect_ast("A ( 1 , 2 )", "(PF (), (P IDENTIFIER), (P CONSTANT), (P CONSTANT))");
    expect_ast("1 . b", "(PF ., (P CONSTANT), IDENTIFIER)");
    expect_ast("1 -> a", "(PF ->, (P CONSTANT), IDENTIFIER)");
}

TEST(ParserSuite, Unary)
{
    expect_ast("++ 1", "(U ++, (P CONSTANT))");
    expect_ast("-- a", "(U --, (P IDENTIFIER))");
    expect_ast("& a", "(U &, (P IDENTIFIER))");
    expect_ast("* a", "(U *, (P IDENTIFIER))");
    expect_ast("+ a", "(U +, (P IDENTIFIER))");
    expect_ast("- a", "(U -, (P IDENTIFIER))");
    expect_ast("~ a", "(U ~, (P IDENTIFIER))");
    expect_ast("! a", "(U !, (P IDENTIFIER))");
}

TEST(ParserSuite, Binary)
{
    expect_ast("1 * 2", "(B (P CONSTANT), *, (P CONSTANT))");
    expect_ast("1 / 1", "(B (P CONSTANT), /, (P CONSTANT))");
    expect_ast("1 % 1", "(B (P CONSTANT), %, (P CONSTANT))");
    expect_ast("1 + 1", "(B (P CONSTANT), +, (P CONSTANT))");
    expect_ast("1 - 1", "(B (P CONSTANT), -, (P CONSTANT))");
    expect_ast("1 << 1", "(B (P CONSTANT), <<, (P CONSTANT))");
    expect_ast("1 >> 1", "(B (P CONSTANT), >>, (P CONSTANT))");
    expect_ast("1 < 1", "(B (P CONSTANT), <, (P CONSTANT))");
    expect_ast("1 > 1", "(B (P CONSTANT), >, (P CONSTANT))");
    expect_ast("1 <= 1", "(B (P CONSTANT), <=, (P CONSTANT))");
    expect_ast("1 >= 1", "(B (P CONSTANT), >=, (P CONSTANT))");
    expect_ast("1 == 1", "(B (P CONSTANT), ==, (P CONSTANT))");
    expect_ast("1 != 1", "(B (P CONSTANT), !=, (P CONSTANT))");
    expect_ast("1 & 1", "(B (P CONSTANT), &, (P CONSTANT))");
    expect_ast("1 ^ 1", "(B (P CONSTANT), ^, (P CONSTANT))");
    expect_ast("1 | 1", "(B (P CONSTANT), |, (P CONSTANT))");
    expect_ast("1 && 1", "(B (P CONSTANT), &&, (P CONSTANT))");
    expect_ast("1 || 1", "(B (P CONSTANT), ||, (P CONSTANT))");
    expect_ast("1 ? \"abc\" : abc", "(C (P CONSTANT), (P STRING), (P IDENTIFIER))");
}