#include <vector>
#include <istream>

#include "token.h"

class Lexer
{
    private:
        std::vector<pToken> tokens;
        std::istream& input;

    public:
        Lexer(std::istream input);

        // Generate tokens from source input.
        std::vector<pToken> get_tokens();
};