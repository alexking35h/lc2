
#include <memory>


#include "ast.h"
#include "parser.h"

std::shared_ptr<ExprAstNode> AstBuilder::postfix(const ParseNode& node)
{
    const ParseNode *left = &(*node.children[0]);
    const ParseNode *right = &(*node.children[1]);
    auto pe = expr(*node.children[0]);

    while(right->empty == false)
    {
        auto p = right->terminals[0];
        switch(p.get_type())
        {
            case TOK_PLUS_PLUS:
                pe = std::make_shared<PostfixExprAstNode>(
                    PostfixType::INC, pe
                );
                right = &(*right->children[0]);
                break;
            case TOK_MINUS_MINUS:
                pe = std::make_shared<PostfixExprAstNode>(
                    PostfixType::DEC, pe
                );
                right = &(*right->children[0]);
                break;
            case TOK_POINTER_OP:
                pe = std::make_shared<PostfixExprAstNode>(
                    PostfixType::PTR_OP, pe, right->terminals[1]
                );
                right = &(*right->children[0]);
                break;
            case '.':
                pe = std::make_shared<PostfixExprAstNode>(
                    PostfixType::DOT, pe, right->terminals[1]
                );
                right = &(*right->children[0]);
                break;
            case '[':
                pe = std::make_shared<PostfixExprAstNode>(
                    PostfixType::ARRAY,
                    pe,
                    std::list<std::shared_ptr<ExprAstNode>>{expr(*right->children[0])}
                );
                right = &(*right->children[1]);
                break;
            case '(':
                {
                    ParseNode& arglist = *right->children[0];
                    std::list<std::shared_ptr<ExprAstNode>> args;
                    if(!arglist.empty)
                    {
                        args.push_back(expr(*arglist.children[0]));
                        for(ParseNode * a = &(*arglist.children[1]);a->empty == false;a = &(*a->children[1]))
                        {
                            args.push_back(expr(*a->children[0]));
                        }
                    }
                    pe = std::make_shared<PostfixExprAstNode>(PostfixType::CALL, pe, args);
                    right = &(*right->children[1]);
                }
                break;
        }
    }
    return pe;
}

std::shared_ptr<ExprAstNode> AstBuilder::unary(const ParseNode& node)
{
    if(node.terminals.size() == 0)
        return expr(*node.children[0]);
    
    UnaryType type;
    switch(node.terminals[0].get_type())
    {
        case TOK_PLUS_PLUS:
            type = UnaryType::INC;
            break;
        case TOK_MINUS_MINUS:
            type = UnaryType::DEC;
            break;
        case '*':
            type = UnaryType::DEREF;
            break;
        case '&':
            type = UnaryType::ADDROF;
            break;
        case '+':
            type = UnaryType::PLUS;
            break;
        case '-':
            type = UnaryType::MINUS;
            break;
        case '~':
            type = UnaryType::COMPLEMENT;
            break;
        case '!':
            type = UnaryType::NOT;
            break;
    }
    return std::make_shared<UnaryExprAstNode>(type, expr(*node.children[0]));
}

std::shared_ptr<ExprAstNode> AstBuilder::cast(const ParseNode& node)
{
    if(node.terminals.size() == 0)
        return expr(*node.children[0]);
}

std::shared_ptr<ExprAstNode> AstBuilder::multiplicative(const ParseNode& node)
{
    auto left = expr(*node.children[0]);
    
    ParseNode * pn = &(*node.children[1]);
    while(pn->empty == false)
    {
        BinaryType op;
        switch(pn->terminals[0].get_type())
        {
            case '*':
                op = BinaryType::MUL;
                break;
            case '/':
                op = BinaryType::DIV;
                break;
            case '%':
                op = BinaryType::MOD;
                break;
        }
        left = std::make_shared<BinaryExprAstNode>(left, expr(*pn->children[0]), op);
        pn = &(*pn->children[1]);
    }
    return left;
}

std::shared_ptr<ExprAstNode> AstBuilder::additive(const ParseNode& node)
{
    auto left = expr(*node.children[0]);
    
    ParseNode * pn = &(*node.children[1]);
    while(pn->empty == false)
    {
        BinaryType op;
        switch(pn->terminals[0].get_type())
        {
            case '+':
                op = BinaryType::ADD;
                break;
            case '-':
                op = BinaryType::SUB;
                break;
        }
        left = std::make_shared<BinaryExprAstNode>(left, expr(*pn->children[0]), op);
        pn = &(*pn->children[1]);
    }
    return left;
}

std::shared_ptr<ExprAstNode> AstBuilder::expr(const ParseNode& node)
{
    switch(node.type)
    {
        case NT_PRIMARY:
            {
                if(node.terminals.size() == 2 && node.terminals[0].get_type() == '(')
                {
                    return expr(*node.children[0]);
                }
                return std::make_shared<PrimaryExprAstNode>(node.terminals[0]);
            }
        case NT_POSTFIX:
            return postfix(node);
        case NT_UNARY:
            return unary(node);
        case NT_CAST:
            return cast(node);
        case NT_MULTIPLICATIVE:
            return multiplicative(node);
        case NT_ADDITIVE:
            return additive(node);
        case NT_EXPRESSION:
            return expr(*node.children[0]);
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