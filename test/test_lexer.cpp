#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <iostream>
#include <memory>

#include "lexer.h"
#include "error.h"

class MockErrorReporter : public ErrorReporter
{
    public:
        MOCK_METHOD(void, report_error, (int line, const std::string& error), (override));
};

bool operator==(std::vector<std::shared_ptr<Token>> left, std::vector<std::shared_ptr<Token>> right)
{
    if(left.size() != right.size()) return false;

    for(auto lp = left.begin(), rp = right.begin(); lp != left.end();lp++, rp++)
    {
        if(*(*lp) != *(*rp)) return false;
    }
    return true;
}

TEST(TokensSuite, TokenOperators)
{
    Token tok_a(TOK_CHAR, 23, 11, "char");
    EXPECT_EQ(tok_a, Token(TOK_CHAR, 23, 11, "char"));
    EXPECT_NE(tok_a, Token(54, 23, 11, "char"));
    EXPECT_NE(tok_a, Token(TOK_CHAR, 99, 11, "char"));
    EXPECT_NE(tok_a, Token(TOK_CHAR, 23, 111, "char"));
    EXPECT_NE(tok_a, Token(TOK_CHAR, 23, 11, "int"));

    std::vector<Token> tokv_a = {Token(TOK_INT, 33, 22, "int")};
    std::vector<Token> tokv_b = {Token(TOK_INT, 33, 22, "int")};
    EXPECT_EQ(tokv_a, tokv_b);
}

TEST(LexerSuite, EmptyInput)
{
    ErrorReporter reporter;
    std::vector<std::shared_ptr<Token>> expected = {
        std::make_shared<Token>(TOK_EOF, 0, 0, "")
    };
    EXPECT_EQ(Lexer("", reporter).get_tokens(), expected);
}

TEST(LexerSuite, SingleCharacterTokens)
{
    ErrorReporter reporter;
    std::string input = "[](){}.&*+-~!/%<>^|?:;,";
    std::vector<std::shared_ptr<Token>> tokens = Lexer(input, reporter).get_tokens();

    ASSERT_EQ(input.size(), tokens.size()-1);
    for(int i = 0;i < input.size();i++)
    {
        if(Token(input[i], 0, i, input.substr(i, 1)) != *tokens[i])
        {
            ASSERT_TRUE(false);
        }
    }
    ASSERT_EQ(*tokens.back(), Token(TOK_EOF, 0, input.size(), ""));
}

TEST(LexerSuite, Whitespace)
{
    ErrorReporter reporter;
    std::string input = " \t;\r\n!";
    std::vector<std::shared_ptr<Token>> expected = {
        std::make_shared<Token>(';', 0, 2, ";"),
        std::make_shared<Token>('!', 1, 5, "!"),
        std::make_shared<Token>(TOK_EOF, 1, 6, "")
    };
    auto tokens = Lexer(input, reporter).get_tokens();
    EXPECT_EQ(tokens, expected);
}

TEST(LexerSuite, Comments)
{
    ErrorReporter reporter;
    std::string input = "// ignore me\n; // ignore me\n";
    std::vector<std::shared_ptr<Token>> expected = {
        std::make_shared<Token>(';', 1, 13, ";"),
        std::make_shared<Token>(TOK_EOF, 2, 28, "")
    };
    auto tokens = Lexer(input, reporter).get_tokens();
    EXPECT_EQ(tokens, expected);
}

TEST(LexerSuite, DoubleCharacterTokens)
{
    ErrorReporter reporter;
    std::string input = "->++--<<>><=>===!=&&||*=/=%=+=-=&=^=|=";
    std::vector<std::shared_ptr<Token>> expected = {
        std::make_shared<Token>(TOK_POINTER_OP, 0, 0, "->"),
        std::make_shared<Token>(TOK_PLUS_PLUS, 0, 2, "++"),
        std::make_shared<Token>(TOK_MINUS_MINUS, 0, 4, "--"),
        std::make_shared<Token>(TOK_SHIFT_LEFT, 0, 6, "<<"),
        std::make_shared<Token>(TOK_SHIFT_RIGHT, 0, 8, ">>"),
        std::make_shared<Token>(TOK_LE, 0, 10, "<="),
        std::make_shared<Token>(TOK_GE, 0, 12, ">="),
        std::make_shared<Token>(TOK_EQ, 0, 14, "=="),
        std::make_shared<Token>(TOK_NE, 0, 16, "!="),
        std::make_shared<Token>(TOK_AND_OP, 0, 18, "&&"),
        std::make_shared<Token>(TOK_OR_OP, 0, 20, "||"),
        std::make_shared<Token>(TOK_MUL_ASSIGN, 0, 22, "*="),
        std::make_shared<Token>(TOK_DIV_ASSIGN, 0, 24, "/="),
        std::make_shared<Token>(TOK_MOD_ASSIGN, 0, 26, "%="),
        std::make_shared<Token>(TOK_PLUS_ASSIGN, 0, 28, "+="),
        std::make_shared<Token>(TOK_MINUS_ASSIGN, 0, 30, "-="),
        std::make_shared<Token>(TOK_AND_ASSIGN, 0, 32, "&="),
        std::make_shared<Token>(TOK_XOR_ASSIGN, 0, 34, "^="),
        std::make_shared<Token>(TOK_OR_ASSIGN, 0, 36, "|="),
        std::make_shared<Token>(TOK_EOF, 0, 38, "")
    };
    auto tokens = Lexer(input, reporter).get_tokens();
    EXPECT_EQ(tokens, expected);
}

TEST(LexerSuite, TripleCharacterTokens)
{
    ErrorReporter reporter;
    std::string input = ">>=<<=";
    std::vector<std::shared_ptr<Token>> expected = {
        std::make_shared<Token>(TOK_SHIFT_RIGHT_ASSIGN, 0, 0, ">>="),
        std::make_shared<Token>(TOK_SHIFT_LEFT_ASSIGN, 0, 3, "<<="),
        std::make_shared<Token>(TOK_EOF, 0, 6, "")
    };
    auto tokens = Lexer(input, reporter).get_tokens();
    EXPECT_EQ(tokens, expected);
}

TEST(LexerSuite, Keywords)
{
    ErrorReporter reporter;
    std::vector<std::string> inputs = {
        "break",
        "case",
        "char",
        "const",
        "continue",
        "default",
        "do",
        "else",
        "for",
        "goto",
        "if",
        "int",
        "register",
        "return",
        "short",
        "signed",
        "static",
        "struct",
        "switch",
        "union",
        "unsigned",
        "void",
        "while"
    };
    std::vector<TokenType> keywords = {
        TOK_BREAK,
        TOK_CASE,
        TOK_CHAR,
        TOK_CONST,
        TOK_CONTINUE,
        TOK_DEFAULT,
        TOK_DO,
        TOK_ELSE,
        TOK_FOR,
        TOK_GOTO,
        TOK_IF,
        TOK_INT,
        TOK_REGISTER,
        TOK_RETURN,
        TOK_SHORT,
        TOK_SIGNED,
        TOK_STATIC,
        TOK_STRUCT,
        TOK_SWITCH,
        TOK_UNION,
        TOK_UNSIGNED,
        TOK_VOID,
        TOK_WHILE
    };
    for(int i = 0;i < inputs.size();i++)
    {
        auto tokens = Lexer(inputs[i], reporter).get_tokens();
        ASSERT_EQ(*tokens[0], Token(keywords[i], 0, 0, inputs[i]));
        ASSERT_EQ(*tokens[1], Token(TOK_EOF, 0, inputs[i].size(), ""));
    }
}

TEST(LexerSuite, Constants)
{
    ErrorReporter reporter;
    auto tokens = Lexer("1234567890", reporter).get_tokens();
    EXPECT_EQ(*tokens[0], Token(TOK_INTEGER_CONSTANT, 0, 0, "1234567890"));

    tokens = Lexer("0x1234567890abcdef", reporter).get_tokens();
    EXPECT_EQ(*tokens[0], Token(TOK_INTEGER_CONSTANT, 0, 0, "0x1234567890abcdef"));

    tokens = Lexer("\"a string\"", reporter).get_tokens();
    EXPECT_EQ(*tokens[0], Token(TOK_STRING_LITERAL, 0, 0, "\"a string\""));
}

TEST(LexerSuite, Identifiers)
{
    ErrorReporter reporter;
    auto tokens = Lexer("a b _c d9", reporter).get_tokens();
    EXPECT_EQ(*tokens[0], Token(TOK_IDENTIFIER, 0, 0, "a"));
    EXPECT_EQ(*tokens[1], Token(TOK_IDENTIFIER, 0, 2, "b"));
    EXPECT_EQ(*tokens[2], Token(TOK_IDENTIFIER, 0, 4, "_c"));
    EXPECT_EQ(*tokens[3], Token(TOK_IDENTIFIER, 0, 7, "d9"));
    EXPECT_EQ(*tokens[4], Token(TOK_EOF, 0, 9, ""));
}

TEST(LexerSuite, Errors)
{
    MockErrorReporter reporter;

    EXPECT_CALL(reporter, report_error(1, "Invalid character in input: '@'"));
    auto tokens = Lexer("//@\n\"@\"@", reporter).get_tokens();
    std::vector<std::shared_ptr<Token>> expected = {
        std::make_shared<Token>(TOK_STRING_LITERAL, 1, 4, "\"@\""),
        std::make_shared<Token>(TOK_EOF, 1, 8, "")
    };
    EXPECT_EQ(tokens, expected);

    EXPECT_CALL(reporter, report_error(0, "Unterminated string literal"));
    EXPECT_CALL(reporter, report_error(1, "Unterminated string literal"));

    tokens = Lexer("\".\n\".", reporter).get_tokens();
    expected = {std::make_shared<Token>(TOK_EOF, 1, 5, "")};
    EXPECT_EQ(tokens, expected);
}
