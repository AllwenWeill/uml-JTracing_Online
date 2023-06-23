#include "ExprAST.h"
class WhileAST : public ExprAST {
    vector<shared_ptr<ExprAST>> m_exprs;
    shared_ptr<ExprAST> m_conds;

public:
    WhileAST(shared_ptr<ExprAST> conds, vector<shared_ptr<ExprAST>> exprs)
        : m_exprs(exprs),
        m_conds(conds) {}
    ~WhileAST() {}
};