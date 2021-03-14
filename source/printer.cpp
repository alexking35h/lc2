#include <iostream>

#include "ast.h"
#include "printer.h"

std::string PrinterVisitor::print(AstNode& root)
{
    root.accept(*this);
    str << std::endl;
    return str.str();
}

void PrinterVisitor::visit(PrimaryExprAstNode& node)
{
    str << "(P " << node.get_token() << ")";
}

void PrinterVisitor::visit(PostfixExprAstNode& node)
{
    str << "(PF ";
    if(node.get_type() == PostfixType::INC)
    {
        str << "++, ";
        node.get_left()->accept(*this);
        str << ")";
    }
    else if(node.get_type() == PostfixType::DEC)
    {
        str << "--, ";
        node.get_left()->accept(*this);
        str << ")";
    }
}