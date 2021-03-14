#include <iostream>
#include <sstream>

#include "ast.h"

class PrinterVisitor : public AstVisitor
{
    private:
        std::stringstream str;
        
    public:
        std::string print(ExprAstNode&);

        virtual void visit(PrimaryExprAstNode&);
};