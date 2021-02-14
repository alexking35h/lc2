#include <iostream>
#include "lexer.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "error.h"

#include <iostream>

class MockErrorReporter : public ErrorReporter
{
    public:
        MOCK_METHOD(void, report_error, (int line, const std::string& error), (override));
};

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
    std::vector<Token> expected = {Token(TOK_EOF, 0, 0, "")};
    EXPECT_EQ(Lexer("", reporter).get_tokens(), expected);
}

TEST(LexerSuite, SingleCharacterTokens)
{
    ErrorReporter reporter;
    std::string input = "[](){}.&*+-~!/%<>^|?:;,";
    std::vector<Token> tokens = Lexer(input, reporter).get_tokens();

    ASSERT_EQ(input.size(), tokens.size()-1);
    for(int i = 0;i < input.size();i++)
    {
        if(Token(input[i], 0, i, input.substr(i, 1)) != tokens[i])
        {
            ASSERT_TRUE(false);
        }
    }
    ASSERT_EQ(tokens.back(), Token(TOK_EOF, 0, input.size(), ""));
}

TEST(LexerSuite, Whitespace)
{
    ErrorReporter reporter;
    std::string input = " \t;\r\n!";
    std::vector<Token> expected = {
        Token(';', 0, 2, ";"),
        Token('!', 1, 5, "!"),
        Token(TOK_EOF, 1, 6, "")
    };
    auto tokens = Lexer(input, reporter).get_tokens();
    EXPECT_EQ(tokens, expected);
}

TEST(LexerSuite, Comments)
{
    ErrorReporter reporter;
    std::string input = "// ignore me\n; // ignore me\n";
    std::vector<Token> expected = {
        Token(';', 1, 13, ";"),
        Token(TOK_EOF, 2, 28, "")
    };
    auto tokens = Lexer(input, reporter).get_tokens();
    EXPECT_EQ(tokens, expected);
}

TEST(LexerSuite, DoubleCharacterTokens)
{
    ErrorReporter reporter;
    std::string input = "->++--<<>><=>===!=&&||*=/=%=+=-=&=^=|=";
    std::vector<Token> expected = {
        Token(TOK_POINTER_OP, 0, 0, "->"),
        Token(TOK_PLUS_PLUS, 0, 2, "++"),
        Token(TOK_MINUS_MINUS, 0, 4, "--"),
        Token(TOK_SHIFT_LEFT, 0, 6, "<<"),
        Token(TOK_SHIFT_RIGHT, 0, 8, ">>"),
        Token(TOK_LE, 0, 10, "<="),
        Token(TOK_GE, 0, 12, ">="),
        Token(TOK_EQ, 0, 14, "=="),
        Token(TOK_NE, 0, 16, "!="),
        Token(TOK_AND_OP, 0, 18, "&&"),
        Token(TOK_OR_OP, 0, 20, "||"),
        Token(TOK_MUL_ASSIGN, 0, 22, "*="),
        Token(TOK_DIV_ASSIGN, 0, 24, "/="),
        Token(TOK_MOD_ASSIGN, 0, 26, "%="),
        Token(TOK_PLUS_ASSIGN, 0, 28, "+="),
        Token(TOK_MINUS_ASSIGN, 0, 30, "-="),
        Token(TOK_AND_ASSIGN, 0, 32, "&="),
        Token(TOK_XOR_ASSIGN, 0, 34, "^="),
        Token(TOK_OR_ASSIGN, 0, 36, "|="),
        Token(TOK_EOF, 0, 38, "")
    };
    auto tokens = Lexer(input, reporter).get_tokens();
    for(int i = 0;i < expected.size();i++)
    {
        if(expected[i] != tokens[i])
        {
            ASSERT_TRUE(false);
        }
    }
    EXPECT_EQ(tokens, expected);
}

TEST(LexerSuite, TripleCharacterTokens)
{
    ErrorReporter reporter;
    std::string input = ">>=<<=";
    std::vector<Token> expected = {
        Token(TOK_SHIFT_RIGHT_ASSIGN, 0, 0, ">>="),
        Token(TOK_SHIFT_LEFT_ASSIGN, 0, 3, "<<="),
        Token(TOK_EOF, 0, 6, "")
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
        ASSERT_EQ(tokens[0], Token(keywords[i], 0, 0, inputs[i]));
        ASSERT_EQ(tokens[1], Token(TOK_EOF, 0, inputs[i].size(), ""));
    }
}

TEST(LexerSuite, Constants)
{
    ErrorReporter reporter;
    auto tokens = Lexer("1234567890", reporter).get_tokens();
    EXPECT_EQ(tokens[0], Token(TOK_INTEGER_CONSTANT, 0, 0, "1234567890"));

    tokens = Lexer("0x1234567890abcdef", reporter).get_tokens();
    EXPECT_EQ(tokens[0], Token(TOK_INTEGER_CONSTANT, 0, 0, "0x1234567890abcdef"));

    tokens = Lexer("\"a string\"", reporter).get_tokens();
    EXPECT_EQ(tokens[0], Token(TOK_STRING_LITERAL, 0, 0, "\"a string\""));
}

TEST(LexerSuite, Identifiers)
{
    ErrorReporter reporter;
    auto tokens = Lexer("a b _c d9", reporter).get_tokens();
    EXPECT_EQ(tokens[0], Token(TOK_IDENTIFIER, 0, 0, "a"));
    EXPECT_EQ(tokens[1], Token(TOK_IDENTIFIER, 0, 2, "b"));
    EXPECT_EQ(tokens[2], Token(TOK_IDENTIFIER, 0, 4, "_c"));
    EXPECT_EQ(tokens[3], Token(TOK_IDENTIFIER, 0, 7, "d9"));
    EXPECT_EQ(tokens[4], Token(TOK_EOF, 0, 9, ""));
}

// TEST(LexerSuite, Errors)
// {
//     MockErrorReporter reporter;

//     EXPECT_CALL(reporter, report_error(2, "Invalid character in input: '@'"));
//     auto tokens = Lexer("//@\n\"@\"@", reporter).get_tokens();
//     std::vector<Token> expected = {
//         Token(TOK_STRING_LITERAL, 1, 5, "\"@\""),
//         Token(TOK_EOF, 2, 9, "")
//     };
//     EXPECT_EQ(tokens, expected);
// }