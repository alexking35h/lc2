#include <iostream>
#include <sstream>

#include "ast.h"

class PrinterVisitor : public AstVisitor
{
    private:
        std::stringstream str;
        
    public:
        std::string print(AstNode&);

        virtual void visit(PrimaryExprAstNode&) override;
        virtual void visit(PostfixExprAstNode&) override;
        virtual void visit(UnaryExprAstNode&) override;
        virtual void visit(BinaryExprAstNode&) override;
        virtual void visit(TertiaryExprAstNode&) override;
};