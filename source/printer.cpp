#include <iostream>

#include "ast.h"
#include "printer.h"

std::string PrinterVisitor::print(AstNode& root)
{
    root.accept(*this);
    return str.str();
}

void PrinterVisitor::visit(PrimaryExprAstNode& node)
{
    str << "(P " << node.token << ")";
}

void PrinterVisitor::visit(PostfixExprAstNode& node)
{
    str << "(PF ";
    if(node.type == PostfixType::INC)
    {
        str << "++, ";
        node.left->accept(*this);
        str << ")";
    }
    else if(node.type == PostfixType::DEC)
    {
        str << "--, ";
        node.left->accept(*this);
        str << ")";
    }
    else if(node.type == PostfixType::ARRAY)
    {
        str << "[], ";
        node.left->accept(*this);
        str << ", ";
        (*node.right.begin())->accept(*this);
        str << ")";
    }
    else if(node.type == PostfixType::PTR_OP)
    {
        str << "->, ";
        node.left->accept(*this);
        str << ", " << node.identifier << ")";
    }
    else if(node.type == PostfixType::DOT)
    {
        str << "., ";
        node.left->accept(*this);
        str << ", " << node.identifier << ")";
    }
    else if(node.type == PostfixType::CALL)
    {
        str << "(), ";
        node.left->accept(*this);
        for(auto arg : node.right)
        {
            str << ", ";
            arg->accept(*this);
        }
        str << ")";
    }
}