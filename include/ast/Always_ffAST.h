#include "ExprAST.h"
class Always_ffAST : public ExprAST{
    shared_ptr<ExprAST> m_flip_flop;
    shared_ptr<ExprAST> Exprs;
public:
    Always_ffAST(shared_ptr<ExprAST> flip_flop, shared_ptr<ExprAST> exprs)
        : m_flip_flop(flip_flop),
        Exprs(exprs) {}
    ~Always_ffAST() {}
};
