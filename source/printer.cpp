#include <iostream>

#include "ast.h"
#include "printer.h"

std::string PrinterVisitor::print(ExprAstNode& root)
{
    root.accept(*this);
    str << std::endl;
    return str.str();
}

void PrinterVisitor::visit(PrimaryExprAstNode& node)
{
    str << node.get_token();
}