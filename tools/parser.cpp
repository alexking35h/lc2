
#include <list>
#include <map>

typedef enum
{
    NT_PRIMARY,
    NT_EXPR,
    NT_EXPR_END
} NonTerminal;

typedef enum
{
    TERMINAL,
    NONTERMINAL,
    ACTION
} PeType;

typedef struct
{
    PeType type;
    union 
    {
        int token;
        NonTerminal nt;
    };
} Pe;

std::map<int, std::list<Pe>> table[] = 
{
    {
        {0, {{TERMINAL, .token=12}, {NONTERMINAL, .nt=NT_PRIMARY}}},
        {1, {{TERMINAL, .token=14}, {NONTERMINAL, .nt=NT_EXPR}}}
    },
    {
        {0, {{TERMINAL, .token=12}, {NONTERMINAL, .nt=NT_PRIMARY}}},
        {1, {{TERMINAL, .token=14}, {NONTERMINAL, .nt=NT_EXPR}}}
    }
};