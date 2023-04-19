#ifndef _NUMBEREXPRAST_H
#define _NUMBEREXPRAST_H


#include "ExprAST.h"
class NumberExprAST : public ExprAST {
    double Val;

public:
    NumberExprAST(double Val) : Val(Val) {}
    ~NumberExprAST(){}
};
#endif // !_NUMBEREXPRAST_H