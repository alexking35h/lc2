#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <regex>
#include <sstream>

#include "lexer.h"

const std::string SingleCharacterTokens = "[](){}.~?:;,";
const std::string DoubleCharacterTokens = "-+=!&|*/%&^";
const std::string TripleCharacterTokens = "<>";
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

char Lexer::peek()
{
    if((position+1) >= input.size()) return '\0';
    return input[position+1];
}

bool Lexer::at_end()
{
    return position >= input.size();
}

bool Lexer::match(char c)
{
    if(!at_end() && input[position] == c)
    {
        position++;
        return true;
    }
    return false;
}

Token Lexer::get_next_token()
{
    char current = input[position];

    if(SingleCharacterTokens.find(current) != std::string::npos)
    {
        int p = position++;
        return Token(current, line, p, input.substr(p, 1));
    }
    if(DoubleCharacterTokens.find(current) != std::string::npos)
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
    if(TripleCharacterTokens.find(current) != std::string::npos)
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
    if(current == '"')
    {
        position++;
        while(!at_end() && input[position++] != '"');
        return Token(
            TOK_STRING_LITERAL,
            line,
            p,
            input.substr(p, position-p)
        );
    }
    if(current == '0' && (peek() == 'x' || peek() == 'X'))
    {
        position++;
        position++;
        while(!at_end() && std::string("1234567890abcdefABCDEF").find(input[position++]) != std::string::npos);
        return Token(
            TOK_INTEGER_CONSTANT,
            line,
            p,
            input.substr(p, position-p)
        );    
    }
    if(std::string("1234567890").find(input[position]) != std::string::npos)
    {
        while(!at_end() && std::string("1234567890").find(input[position++]) != std::string::npos);
        return Token(
            TOK_INTEGER_CONSTANT,
            line,
            p,
            input.substr(p, position-p)
        );
    }

    if(std::string("qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM_").find(input[position]) == std::string::npos)
    {
        return Token();
        // Invalid character.
    }

    while(!at_end())
    {
        int ip = ++position;
        if(std::string("1234567890").find(input[ip]) != std::string::npos) continue;
        if(std::string("qwertyuiopasdfghjklzxcvbnm_").find(input[ip]) != std::string::npos) continue;
        if(std::string("QWERTYUIOPASDFGHJKLZXCVBNM").find(input[ip]) != std::string::npos) continue;
        break;
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
                if(peek() != '/') return;
                while(peek() != '\n') position++;
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

        tokens.push_back(get_next_token());
    }
    tokens.push_back(Token(TOK_EOF, line, position, ""));
    return tokens;
}