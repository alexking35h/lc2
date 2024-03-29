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
    str << "(P " << *node.token << ")";
}

void PrinterVisitor::visit(BinaryExprAstNode& node)
{
    str << "(B ";
    node.left->accept(*this);
    switch(node.op)
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
        case BinaryType::ADD:
            str << ", +, ";
            break;
        case BinaryType::SUB:
            str << ", -, ";
            break;
        case BinaryType::SHIFT_LEFT:
            str << ", <<, ";
            break;
        case BinaryType::SHIFT_RIGHT:
            str << ", >>, ";
            break;
        case BinaryType::LT:
            str << ", <, ";
            break;
        case BinaryType::GT:
            str << ", >, ";
            break;
        case BinaryType::LE:
            str << ", <=, ";
            break;
        case BinaryType::GE:
            str << ", >=, ";
            break;
        case BinaryType::EQ:
            str << ", ==, ";
            break;
        case BinaryType::NE:
            str << ", !=, ";
            break;
        case BinaryType::BITWISE_AND:
            str << ", &, ";
            break;
        case BinaryType::BITWISE_EXCL_OR:
            str << ", ^, ";
            break;
        case BinaryType::BITWISE_INCL_OR:
            str << ", |, ";
            break;
        case BinaryType::LOGICAL_AND_OP:
            str << ", &&, ";
            break;
        case BinaryType::LOGICAL_OR_OP:
            str << ", ||, ";
            break;
    }
    node.right->accept(*this);
    str << ")";
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

void PrinterVisitor::visit(TertiaryExprAstNode& node)
{
    str << "(T ";
    node.conditional->accept(*this);
    str << ", ";
    node.left->accept(*this);
    str << ", ";
    node.right->accept(*this);
    str << ")";
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
        str << ", " << *node.identifier << ")";
    }
    else if(node.type == PostfixType::DOT)
    {
        str << "., ";
        node.left->accept(*this);
        str << ", " << *node.identifier << ")";
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

void PrinterVisitor::visit(AssignExprAstNode& node)
{
    str << "(A ";
    node.left->accept(*this);
    switch(node.type)
    {
        case AssignExprType::ASSIGN:
            str << ", =, ";
            break;
        case AssignExprType::PLUS:
            str << ", +, ";
            break;
        case AssignExprType::MINUS:
            str << ", -, ";
            break;
        case AssignExprType::MUL:
            str << ", *, ";
            break;
        case AssignExprType::DIV:
            str << ", /, ";
            break;
        case AssignExprType::MOD:
            str << ", %, ";
            break;
        case AssignExprType::XOR:
            str << ", ^, ";
            break;
        case AssignExprType::SHIFT_LEFT:
            str << ", <<, ";
            break;
        case AssignExprType::SHIFT_RIGHT:
            str << ", >>, ";
            break;
        case AssignExprType::OR:
            str << ", |, ";
            break;
        case AssignExprType::AND:
            str << ", &, ";
            break;
    }
    node.right->accept(*this);
    str << ")";
}

void PrinterVisitor::visit(ExprAstNode&)
{}

void PrinterVisitor::visit(DeclAstNode&)
{}