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
        Token token;

    public:
        PrimaryExprAstNode(Token t) : token(t) {}
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
        Token identifier;
        
    public:
        PostfixExprAstNode(
            PostfixType type,
            std::shared_ptr<ExprAstNode> left
        ) : type(type)
          , left(left) {}
        PostfixExprAstNode(
            PostfixType type,
            std::shared_ptr<ExprAstNode> left,
            Token identifier
        ) : type(type)
          , left(left)
          , identifier(identifier) {}
        PostfixExprAstNode(
            PostfixType type,
            std::shared_ptr<ExprAstNode> left,
            std::list<std::shared_ptr<ExprAstNode>> right
        ) : type(type)
          , left(left)
          , right(right) {}
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