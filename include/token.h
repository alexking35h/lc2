#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <memory>

/*
 - Missing: 
 - auto
 - double
 - enum
 - extern
 - float
 - inline
 - long
 - restrict
 - sizeof
 - typedef
 - volatile
 - _Bool
 - _Complex
 - _Imaginary
*/

typedef enum TokenType : int
{
    TOK_NAT,

    // Single character tokens (+,-,/,%,etc) do not have
    // a TokenType. Token stores the ascii character directly.
    // (all values less than 127).

    // Keywords (A.1.2)
    TOK_BREAK = 128,
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
    TOK_WHILE,

    // Identifiers (A.1.3)
    TOK_IDENTIFIER,

    // Constants (A.1.5) and string literals (A.1.6)
    TOK_INTEGER_CONSTANT,
    TOK_STRING_LITERAL,

    // Punctuation
    TOK_POINTER_OP,
    TOK_PLUS_PLUS,
    TOK_MINUS_MINUS,
    TOK_SHIFT_LEFT,
    TOK_SHIFT_RIGHT,
    TOK_LE,
    TOK_GE,
    TOK_EQ,
    TOK_NE,
    TOK_AND_OP,
    TOK_OR_OP, TOK_MUL_ASSIGN, TOK_DIV_ASSIGN, TOK_MOD_ASSIGN,
    TOK_PLUS_ASSIGN, TOK_MINUS_ASSIGN, TOK_SHIFT_LEFT_ASSIGN,
    TOK_SHIFT_RIGHT_ASSIGN, TOK_AND_ASSIGN, TOK_XOR_ASSIGN,
    TOK_OR_ASSIGN,

    // End of file.
    TOK_EOF
} TokenType;

class Token
{
    private:
        int type;
        int line;
        int position;
        std::string lexeme;

    public:
        Token();
        Token(int, int, int, std::string);

        inline int get_type() { return type; }
        inline int get_line() { return line; }
        inline int get_position() { return position; }
        inline std::string get_lexeme() { return lexeme; }

        inline bool operator==(const Token token) const
        {
            return type == token.type
                && line == token.line
                && position == token.position
                && lexeme == token.lexeme;
        }
        inline bool operator!=(const Token token) const
        {
            return !(*this  == token);
        }
};

#endif