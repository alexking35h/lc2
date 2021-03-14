#ifndef AST_H_ 
#define AST_H_ 1
#include <memory>
#include <list>

#include "token.h"

class PrimaryExprAstNode;
class BinaryExprAstNode;
class UnaryExprAstNode;
class TertiaryExprAstNode;
class PostfixExprAstNode;
class AssignExprAstNode;
class ExprAstNode;

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
    private:
        Token token;

    public:
        inline Token get_token() {
            return token;
        }
        inline PrimaryExprAstNode(Token t) : token(t) {}
        virtual void accept(AstVisitor&) override;
};

class PostfixExprAstNode : public ExprAstNode
{
    private:
        Token token;
        union {
            std::shared_ptr<ExprAstNode> expression;
            std::list<std::shared_ptr<ExprAstNode>> expressions;
            Token identifier;
        };
        std::shared_ptr<PrimaryExprAstNode> next;
        
    public:
        virtual void accept(AstVisitor&) override;
};

class BinaryExprAstNode : public ExprAstNode
{
    private:
        std::shared_ptr<ExprAstNode> left, right;
        Token op;
    
    public:
        inline std::shared_ptr<ExprAstNode> get_left() {
            return left;
        }
        inline std::shared_ptr<ExprAstNode> get_right() {
            return right;
        }
        inline Token get_op() {
            return op;
        }
        inline BinaryExprAstNode(
            std::shared_ptr<ExprAstNode> left,
            std::shared_ptr<ExprAstNode> right,
            Token op
        ) : left(left), right(right), op(op) {}
        virtual void accept(AstVisitor&) override;
};

class UnaryExprAstNode : public ExprAstNode
{
    public:
        virtual void accept(AstVisitor&) override;
};

class TertiaryExprAstNode : public ExprAstNode
{
    public:
        virtual void accept(AstVisitor&) override;
};

class AssignExprAstNode : public ExprAstNode
{
    public:
        virtual void accept(AstVisitor&) override;
};

#endif