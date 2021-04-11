// AST Printer class - generate string representation from an AST.

#ifndef _PRINTER_H_
#define _PRINTER_H_

#include <sstream>

#include "ast.h"

// This class traverses the AST in an inorder walk, and generates a 
// string representation for each node:
// - Expression nodes:
//   (P/B/U/T/PF/A/E ...)
class PrinterVisitor : public AstVisitor
{
    private:
        std::stringstream str;
        
    public:
        std::string print(AstNode&);

        virtual void visit(PrimaryExprAstNode&) override;
        virtual void visit(BinaryExprAstNode&) override;
        virtual void visit(UnaryExprAstNode&) override;
        virtual void visit(TertiaryExprAstNode&) override;
        virtual void visit(PostfixExprAstNode&) override;
        virtual void visit(AssignExprAstNode&) override;
        virtual void visit(ExprAstNode&) override;
};

#endif