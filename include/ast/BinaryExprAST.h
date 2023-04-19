#ifndef _BINARYEXPRAST_H
#define _BINARYEXPRAST_H

#include "ExprAST.h"
class BinaryExprAST : public ExprAST {
  string Op;
  std::shared_ptr<ExprAST> LHS, RHS;

public:
    BinaryExprAST(string op, std::shared_ptr<ExprAST> LHS,
                std::shared_ptr<ExprAST> RHS)
    : Op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
    ~BinaryExprAST(){}
};

#endif // !_BINARYEXPRAST_H