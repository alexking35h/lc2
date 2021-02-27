#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <regex>
#include <sstream>

#include "lexer.h"

const std::string SingleCharacters = "[](){}.~?:;,";
const std::string DoubleCharacters = "-+=!&|*/%&^";
const std::string TripleCharacters = "<>";
const std::string DigitCharacters = "1234567890";
const std::string HexCharacters = "1234567890ABCDEFabcdef";
const std::string AlphaCharacters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_";
const std::vector<std::string> KeywordTokens = {
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
const std::vector<TokenType> KeywordTypes = {
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

Lexer::Lexer(const std::string& input, ErrorReporter& reporter)
    : tokens()
    , input(input)
    , position(0)
    , line(0)
    , error_reporter(reporter)
{
}

bool Lexer::consume_constant()
{
    if(input[position] == '0' && (peek_next() == 'x' || peek_next() == 'X'))
    {
        position++;
        position++;
        while(std::string("1234567890abcdefABCDEF").find(input[position++]) != std::string::npos);
        return true;
    }
    if(std::string("1234567890").find(input[position]) != std::string::npos)
    {
        while(std::string("1234567890").find(input[position++]) != std::string::npos);
        return true;
    }
    return false;
}

bool Lexer::consume_string_literal()
{
    if(input[position] == '"')
    {
        position++;
        while(true)
        {
            if(at_end() || input[position] == '\n') throw std::string("Unterminated string literal");
            if(input[position++] == '"') break;
        }
        return true;
    }
    return false;
}

Token Lexer::get_next_token()
{
    char current = input[position];

    if(SingleCharacters.find(current) != std::string::npos)
    {
        int p = position++;
        return Token(current, line, p, input.substr(p, 1));
    }
    if(DoubleCharacters.find(current) != std::string::npos)
    {
        int p = position++;
        int type;
        switch(current)
        {
            case '-':
                {
                    if(match('-')) type = TOK_MINUS_MINUS;
                    else if(match('=')) type = TOK_MINUS_ASSIGN;
                    else type = match('>') ? TOK_POINTER_OP : '-';
                }
                break;
            case '+':
                {
                    if(match('+')) type = TOK_PLUS_PLUS;
                    else type = match('=') ? TOK_PLUS_ASSIGN : '+';
                }
                break;
            case '<':
                {
                    if(match('<')) type = TOK_SHIFT_LEFT;
                    else type = match('=') ? TOK_LE : '<';
                }
                break;
            case '>':
                {
                    if(match('>')) type = TOK_SHIFT_RIGHT;
                    else type = match('=') ? TOK_GE : '>';
                }
                break;
            case '&':
                {
                    if(match('&')) type = TOK_AND_OP;
                    else type = match('=') ? TOK_AND_ASSIGN : '&';
                }
                break;
            case '|':
                {
                    if(match('|')) type = TOK_OR_OP;
                    else type = match('=') ? TOK_OR_ASSIGN : '|';
                }
                break;
            case '!':
                type = match('=') ? TOK_NE : '!';
                break;
            case '*':
                type = match('=') ? TOK_MUL_ASSIGN : '*';
                break;
            case '/':
                type = match('=') ? TOK_DIV_ASSIGN : '/';
                break;
            case '%':
                type = match('=') ? TOK_MOD_ASSIGN : '%';
                break;
            case '^':
                type = match('=') ? TOK_XOR_ASSIGN : '^';
                break;
            case '=':
                type = match('=') ? TOK_EQ : '=';
                break;
            default:
                break;
        }
        return Token(type, line, p, input.substr(p, position-p));
    }
    if(TripleCharacters.find(current) != std::string::npos)
    {
        int p = position++;
        if(current == '<')
        {
            if(match('<'))
            {
                TokenType type = match('=') ? TOK_SHIFT_LEFT_ASSIGN : TOK_SHIFT_LEFT;
                return Token(type, line, p, input.substr(p, position-p));
            }
            else if(match('='))
            {
                return Token(TOK_LE, line, p, input.substr(p, 2));
            }
                return Token('<', line, p, input.substr(p, 1));
        }
        else if(current == '>')
        {
            if(match('>'))
            {
                TokenType type = match('=') ? TOK_SHIFT_RIGHT_ASSIGN : TOK_SHIFT_RIGHT;
                return Token(type, line, p, input.substr(p, position-p));
            }
            else if(match('='))
            {
                return Token(TOK_GE, line, p, input.substr(p, 2));
            }
            return Token('>', line, p, input.substr(p, 1));
        }
    } 

    int p = position;
    if(consume_string_literal())
    {
        return Token(TOK_STRING_LITERAL, line, p, input.substr(p, position-p));
    }
    if(consume_constant())
    {
        return Token(TOK_INTEGER_CONSTANT, line, p, input.substr(p, position-p));
    }

    if(AlphaCharacters.find(input[position]) != std::string::npos)
    {
        position++;
        while(true)
        {
            if(AlphaCharacters.find(input[position]) != std::string::npos) position++;
            else if(DigitCharacters.find(input[position]) != std::string::npos) position++;
            else break;
        }
    }
    else
    {
        std::stringstream err;
        err << "Invalid character in input: '" << input[position] << "'";
        position++;
        throw err.str();
    }

    std::string lexeme = input.substr(p, position - p);

    for(int i = 0;i != KeywordTokens.size();i++)
    {
        if(KeywordTokens[i] == lexeme)
        {
            return Token(KeywordTypes[i], line, p, lexeme);
        }
    }
    return Token(TOK_IDENTIFIER, line, p, lexeme);
}

void Lexer::skip_whitespace_comments()
{
    while(!at_end())
    {
        switch(input[position])
        {
            case '\n':
                line++;
            case ' ':
            case '\t':
            case '\r':
                break;
            case '/':
            {
                if(peek_next() != '/') return;
                while(peek_next() != '\n') position++;
                break;
            }
            default:
                return;
        }
        position++;
    }
}

std::vector<Token> Lexer::get_tokens()
{
    std::vector<Token> tokens;
    position = 0;
    while(true)
    {
        skip_whitespace_comments();

        if(at_end()) break;
        
        try
        {
            tokens.push_back(get_next_token());
        }
        catch (std::string errmsg)
        {
            error_reporter.report_error(line, errmsg);
        }
    }
    tokens.push_back(Token(TOK_EOF, line, position, ""));
    return tokens;
}