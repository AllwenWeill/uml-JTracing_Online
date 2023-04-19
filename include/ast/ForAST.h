#include "ExprAST.h"
#include "BinaryExprAST.h"
#include "CmpExprAST.h"
class ForAST : public ExprAST {
    shared_ptr<ExprAST> Exprs;
    shared_ptr<ExprAST> m_init;
    shared_ptr<ExprAST> m_cmp;
    shared_ptr<ExprAST> m_step;
public:
    ForAST(shared_ptr<ExprAST> exprs, shared_ptr<ExprAST> init, shared_ptr<ExprAST> cmp, shared_ptr<ExprAST> step)
        : Exprs(exprs),
        m_init(init),
        m_cmp(cmp),
        m_step(step) {}
    ~ForAST() {}
};