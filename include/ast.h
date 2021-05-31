// Abstract Syntax Tree (AST)
//
// An AST is used internally to represent the source program during semantic analysis.
//
// This file provides class declarations for AST node types, AstBuilder class, and AstVisitor 
// base class. AstBuilder builds an AST representation from a parse tree. AstVisitor provides
// abstract methods for walking an AST.

#ifndef AST_H_ 
#define AST_H_

#include <memory>
#include <list>

#include "token.h"
#include "parser.h"

class PrimaryExprAstNode;
class BinaryExprAstNode;
class UnaryExprAstNode;
class TertiaryExprAstNode;
class PostfixExprAstNode;
class AssignExprAstNode;
class ExprAstNode;
class AstNode;
class DeclAstNode;

// AST visitor abstract base class.
class AstVisitor
{
    public:
        virtual void visit(PrimaryExprAstNode&) = 0;
        virtual void visit(BinaryExprAstNode&) = 0;
        virtual void visit(UnaryExprAstNode&) = 0;
        virtual void visit(TertiaryExprAstNode&) = 0;
        virtual void visit(PostfixExprAstNode&) = 0;
        virtual void visit(AssignExprAstNode&) = 0;
        virtual void visit(ExprAstNode&) = 0;
        virtual void visit(DeclAstNode&) = 0;
};

// AST builder class
//
// This class creates an AST representation from a parse tree.
class AstBuilder
{
    private:
        std::shared_ptr<ExprAstNode> expr(const ParseNode&);
        std::shared_ptr<ExprAstNode> postfix(const ParseNode&);
        std::shared_ptr<ExprAstNode> unary(const ParseNode&);
        std::shared_ptr<ExprAstNode> cast(const ParseNode&);
        std::shared_ptr<ExprAstNode> binary(const ParseNode&);
        std::shared_ptr<ExprAstNode> tertiary(const ParseNode&);
        std::shared_ptr<ExprAstNode> assignment(const ParseNode&);
        std::shared_ptr<DeclAstNode> declaration(const ParseNode&);
    public:
        std::shared_ptr<AstNode> build(const ParseNode&);
};

// AST node base class.
class AstNode
{
    public:
        virtual void accept(AstVisitor&) = 0;
        virtual ~AstNode();
};

// Expression node (should not be instantiated directly)
class ExprAstNode : public AstNode
{
};

class PrimaryExprAstNode : public ExprAstNode
{
    public:
        const std::shared_ptr<Token> token;

        explicit inline PrimaryExprAstNode(std::shared_ptr<Token> t) : token(t) {}

        virtual void accept(AstVisitor&) override;
};

enum class PostfixType
{
    ARRAY, CALL, PTR_OP, INC, DEC, DOT
};

class PostfixExprAstNode : public ExprAstNode
{
    public:
        const PostfixType type;
        const std::shared_ptr<ExprAstNode> left;

        // ExprAstNode list - for ARRAY, CALL types.
        const std::list<std::shared_ptr<ExprAstNode>> right;

        // identifier token - for PTR_OP, DOT types.
        const std::shared_ptr<Token> identifier;
        
        inline PostfixExprAstNode(
            PostfixType type,
            std::shared_ptr<ExprAstNode> left
        ) : type(type)
          , left(left) {}
        inline PostfixExprAstNode(
            PostfixType type,
            std::shared_ptr<ExprAstNode> left,
            std::shared_ptr<Token> identifier
        ) : type(type)
          , left(left)
          , identifier(identifier) {}
        inline PostfixExprAstNode(
            PostfixType type,
            std::shared_ptr<ExprAstNode> left,
            const std::list<std::shared_ptr<ExprAstNode>>&& right
        ) : type(type)
          , left(left)
          , right(right) {}

        virtual void accept(AstVisitor&) override;
};

enum class UnaryType
{
    INC, DEC, ADDROF, DEREF, PLUS, MINUS, COMPLEMENT, NOT
};

class UnaryExprAstNode : public ExprAstNode
{
    public:
        const UnaryType type;
        const std::shared_ptr<ExprAstNode> right;

        inline UnaryExprAstNode(
            UnaryType type,
            std::shared_ptr<ExprAstNode> right
        ) : type(type)
          , right(right) {}

        virtual void accept(AstVisitor&) override;
};

// Binary AST node types
// (For all binary types in the language - except assignments).
enum class BinaryType
{
    MUL, DIV, MOD, ADD, SUB, SHIFT_LEFT, SHIFT_RIGHT, LT, GT,
    LE, GE, EQ, NE, BITWISE_AND, BITWISE_EXCL_OR, BITWISE_INCL_OR,
    LOGICAL_AND_OP, LOGICAL_OR_OP
};

class BinaryExprAstNode : public ExprAstNode
{
    public:
        const std::shared_ptr<ExprAstNode> left, right;
        const BinaryType op;
    
        inline BinaryExprAstNode(
            std::shared_ptr<ExprAstNode> left,
            std::shared_ptr<ExprAstNode> right,
            BinaryType op
        ) : left(left)
          , right(right)
          , op(op) {}

        virtual void accept(AstVisitor&) override;
};

// Tertiary AST node (i.e., conditional expression).
class TertiaryExprAstNode : public ExprAstNode
{
    public:
        const std::shared_ptr<ExprAstNode> conditional, left, right;

        inline TertiaryExprAstNode(
            std::shared_ptr<ExprAstNode> conditional,
            std::shared_ptr<ExprAstNode> left,
            std::shared_ptr<ExprAstNode> right
        ) : conditional(conditional)
        , left(left)
        , right(right) {}

        virtual void accept(AstVisitor&) override;
};

enum class AssignExprType
{
    ASSIGN, PLUS, MINUS, MUL, DIV, MOD, XOR, AND, OR, SHIFT_LEFT, SHIFT_RIGHT
};

// Assignment expressions (=, -=, *=, etc.)
class AssignExprAstNode : public ExprAstNode
{
    public:
        const AssignExprType type;
        const std::shared_ptr<ExprAstNode> left, right;

        inline AssignExprAstNode(
            std::shared_ptr<ExprAstNode> left,
            AssignExprType type,
            std::shared_ptr<ExprAstNode> right
        ) : type(type)
          , left(left)
          , right(right) {}

        virtual void accept(AstVisitor&) override;
};

// Declaration
class DeclAstNode : public AstNode
{
    public:
        virtual void accept(AstVisitor&) override;
};

#endif