#include <iostream>
#include <sstream>

#include "ast.h"

class PrinterVisitor : public AstVisitor
{
    private:
        std::stringstream str;
        
    public:
        std::string print(AstNode&);

        virtual void visit(PrimaryExprAstNode&);
        virtual void visit(PostfixExprAstNode&);
        virtual void visit(UnaryExprAstNode&);
};