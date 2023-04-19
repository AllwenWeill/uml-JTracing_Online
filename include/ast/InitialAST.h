#include "ExprAST.h"
class InitialAST : public ExprAST {
    shared_ptr<ExprAST> Exprs;
public:
    InitialAST(shared_ptr<ExprAST> exprs)
        : Exprs(exprs) {}
    ~InitialAST() {}
};