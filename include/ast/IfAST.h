#include "ExprAST.h"
class IfAST : public ExprAST {
	shared_ptr<ExprAST> m_cond;
	vector<shared_ptr<ExprAST>> m_exprs;
public:
	IfAST(shared_ptr<ExprAST> cond, vector<shared_ptr<ExprAST>> exprs)
		:m_cond(cond),
		m_exprs(exprs) {}
	~IfAST() {}
};