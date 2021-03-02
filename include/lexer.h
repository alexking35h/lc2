#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <istream>

#include "token.h"
#include "error.h"

class Lexer
{
    private:
        std::vector<Token> tokens;
        std::string input;
        int position;
        int line;
        ErrorReporter& error_reporter;

        inline char peek() {
            return position >= input.size() ? '\0' : input[position];
        }
        inline bool at_end() {
            return position >= input.size();
        }
        inline char peek_next() {
            return (position+1) >= input.size() ? '\0' : input[position+1];
        }
        inline bool match(char c)
        {
            if(!at_end() && peek() == c)
            {
                position++;
                return true;
            }
            return false;
        }
        void skip_whitespace_comments();
        bool consume_string_literal();
        bool consume_constant();
        Token get_next_token();

    public:
        Lexer(const std::string&, ErrorReporter&);

        // Generate tokens from source input.
        std::vector<Token> get_tokens();
};

#endif