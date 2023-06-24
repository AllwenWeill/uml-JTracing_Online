#include "ExprAST.h"
class ElseAST : public ExprAST {
    shared_ptr<ExprAST> m_cond;
    vector<shared_ptr<ExprAST>> m_exprs;
public:
    ElseAST(shared_ptr<ExprAST> cond, vector<shared_ptr<ExprAST>> exprs)
        :m_cond(cond),
        m_exprs(exprs) {}
    ~ElseAST() {}
};