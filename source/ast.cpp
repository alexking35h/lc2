
#include <memory>

#include "ast.h"
#include "parser.h"

std::shared_ptr<ExprAstNode> AstBuilder::postfix(const ParseNode& node)
{
    if(node.children[1]->empty == true)
    {
        return expr(*node.children[0]);
    }
    if(node.children[1]->terminals[0].get_type() == TOK_PLUS_PLUS)
    {
        return std::make_shared<PostfixExprAstNode>(
            PostfixType::INC, expr(*node.children[0]));
    }
    if(node.children[1]->terminals[0].get_type() == TOK_MINUS_MINUS)
    {
        return std::make_shared<PostfixExprAstNode>(
            PostfixType::DEC, expr(*node.children[0]));
    }
}

std::shared_ptr<ExprAstNode> AstBuilder::expr(const ParseNode& node)
{
    if(node.type == NT_EXPRESSION)
    {
        return expr(*node.children[0]);
    }
    if(node.type == NT_PRIMARY)
    {
        return std::make_shared<PrimaryExprAstNode>(node.terminals[0]);
    }
    if(node.type == NT_POSTFIX)
    {
        return postfix(node);
    }
}

std::shared_ptr<AstNode> AstBuilder::build(const ParseNode& node)
{
    if(node.type == NT_ROOT)
    {
        return expr(*node.children[0]);
    }
}

void AstVisitor::visit(PrimaryExprAstNode& node)
{

}

void AstVisitor::visit(BinaryExprAstNode& node)
{
    node.get_left()->accept(*this);
    node.get_right()->accept(*this);
}

void AstVisitor::visit(UnaryExprAstNode&)
{

}

void AstVisitor::visit(TertiaryExprAstNode&)
{

}

void AstVisitor::visit(PostfixExprAstNode&)
{

}

void AstVisitor::visit(AssignExprAstNode&)
{

}

void AstVisitor::visit(ExprAstNode&)
{
    
}

void PrimaryExprAstNode::accept(AstVisitor& v)
{
    v.visit(*this);
}

void BinaryExprAstNode::accept(AstVisitor& v)
{
    v.visit(*this);
}

void UnaryExprAstNode::accept(AstVisitor& v)
{
    v.visit(*this);
}

void TertiaryExprAstNode::accept(AstVisitor& v)
{
    v.visit(*this);
}

void PostfixExprAstNode::accept(AstVisitor& v)
{
    v.visit(*this);
}

void AssignExprAstNode::accept(AstVisitor& v)
{
    v.visit(*this);
}