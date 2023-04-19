#include "ExprAST.h"
class Always_combAST : public ExprAST {
    shared_ptr<ExprAST> Exprs;
public:
    Always_combAST(shared_ptr<ExprAST> exprs)
        : Exprs(exprs) {}
    ~Always_combAST() {}
};