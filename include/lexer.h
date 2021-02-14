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

        char peek();
        bool match(char);
        bool at_end();
        void skip_whitespace_comments();
        Token get_next_token();

    public:
        Lexer(const std::string&, ErrorReporter&);

        // Generate tokens from source input.
        std::vector<Token> get_tokens();
};