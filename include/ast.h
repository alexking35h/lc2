#ifndef AST_H_ 
#define AST_H_ 1
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

class AstVisitor
{
    public:
        virtual void visit(PrimaryExprAstNode&);
        virtual void visit(BinaryExprAstNode&);
        virtual void visit(UnaryExprAstNode&);
        virtual void visit(TertiaryExprAstNode&);
        virtual void visit(PostfixExprAstNode&);
        virtual void visit(AssignExprAstNode&);
        virtual void visit(ExprAstNode&);
};

class AstBuilder
{
    private:
        std::shared_ptr<ExprAstNode> expr(const ParseNode&);
        std::shared_ptr<ExprAstNode> postfix(const ParseNode&);
        std::shared_ptr<ExprAstNode> unary(const ParseNode&);
        std::shared_ptr<ExprAstNode> cast(const ParseNode&);
        std::shared_ptr<ExprAstNode> binary(const ParseNode&);
        std::shared_ptr<ExprAstNode> tertiary(const ParseNode&);
    public:
        std::shared_ptr<AstNode> build(const ParseNode&);
};

class AstNode
{
    public:
        virtual void accept(AstVisitor&) = 0;
};

class ExprAstNode : public AstNode
{
};

class PrimaryExprAstNode : public ExprAstNode
{
    public:
        std::shared_ptr<Token> token;

    public:
        explicit PrimaryExprAstNode(std::shared_ptr<Token> t) : token(t) {}
        virtual void accept(AstVisitor&) override;
};

enum class PostfixType
{
    ARRAY, CALL, PTR_OP, INC, DEC, DOT
};

class PostfixExprAstNode : public ExprAstNode
{
    public:
        PostfixType type;
        std::shared_ptr<ExprAstNode> left;
        std::list<std::shared_ptr<ExprAstNode>> right;
        std::shared_ptr<Token> identifier;
        
    public:
        PostfixExprAstNode(
            PostfixType type,
            std::shared_ptr<ExprAstNode> left
        ) : type(type)
          , left(left) {}
        PostfixExprAstNode(
            PostfixType type,
            std::shared_ptr<ExprAstNode> left,
            std::shared_ptr<Token> identifier
        ) : type(type)
          , left(left)
          , identifier(identifier) {}
        PostfixExprAstNode(
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
        UnaryType type;
        std::shared_ptr<ExprAstNode> right;

    public:
        inline UnaryExprAstNode(
            UnaryType type,
            std::shared_ptr<ExprAstNode> right
        ) : type(type)
          , right(right) {}
        virtual void accept(AstVisitor&) override;
};

enum class BinaryType
{
    MUL, DIV, MOD, ADD, SUB, SHIFT_LEFT, SHIFT_RIGHT, LT, GT,
    LE, GE, EQ, NE, BITWISE_AND, BITWISE_EXCL_OR, BITWISE_INCL_OR,
    LOGICAL_AND_OP, LOGICAL_OR_OP
};

class BinaryExprAstNode : public ExprAstNode
{
    private:
        std::shared_ptr<ExprAstNode> left, right;
        BinaryType op;
    
    public:
        inline std::shared_ptr<ExprAstNode> get_left() {
            return left;
        }
        inline std::shared_ptr<ExprAstNode> get_right() {
            return right;
        }
        inline BinaryType get_op() {
            return op;
        }
        inline BinaryExprAstNode(
            std::shared_ptr<ExprAstNode> left,
            std::shared_ptr<ExprAstNode> right,
            BinaryType op
        ) : left(left), right(right), op(op) {}
        virtual void accept(AstVisitor&) override;
};

class TertiaryExprAstNode : public ExprAstNode
{
    public:
        std::shared_ptr<ExprAstNode> conditional, left, right;

    public:
        inline TertiaryExprAstNode(
            std::shared_ptr<ExprAstNode> conditional,
            std::shared_ptr<ExprAstNode> left,
            std::shared_ptr<ExprAstNode> right
        ) : conditional(conditional)
        , left(left)
        , right(right) {}
        virtual void accept(AstVisitor&) override;
};

class AssignExprAstNode : public ExprAstNode
{
    public:
        virtual void accept(AstVisitor&) override;
};

#endif