
#include "ast.h"

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