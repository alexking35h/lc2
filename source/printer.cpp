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

void PrinterVisitor::visit(UnaryExprAstNode& node)
{
    str << "(U ";
    if(node.type == UnaryType::INC)
    {
        str << "++, ";
        node.right->accept(*this);
    }
    if(node.type == UnaryType::DEC)
    {
        str << "--, ";
        node.right->accept(*this);
    }
    if(node.type == UnaryType::ADDROF) {
        str << "&, ";
        node.right->accept(*this);
    }
    if(node.type == UnaryType::DEREF) {
        str << "*, ";
        node.right->accept(*this);
    }
    if(node.type == UnaryType::PLUS) {
        str << "+, ";
        node.right->accept(*this);
    }
    if(node.type == UnaryType::MINUS) {
        str << "-, ";
        node.right->accept(*this);
    }
    if(node.type == UnaryType::COMPLEMENT) {
        str << "~, ";
        node.right->accept(*this);
    }
    if(node.type == UnaryType::NOT) {
        str << "!, ";
        node.right->accept(*this);
    }
    str <<")";
}

void PrinterVisitor::visit(BinaryExprAstNode& node)
{
    str << "(B ";
    node.get_left()->accept(*this);
    switch(node.get_op())
    {
        case BinaryType::MUL:
            str << ", *, ";
            break;
        case BinaryType::DIV:
            str << ", /, ";
            break;
        case BinaryType::MOD:
            str << ", %, ";
            break;
    }
    node.get_right()->accept(*this);
    str << ")";
}