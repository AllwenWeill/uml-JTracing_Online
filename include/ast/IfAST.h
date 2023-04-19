#include "ExprAST.h"
class IfAST : public ExprAST {
	shared_ptr<ExprAST> m_cond; 
	shared_ptr<ExprAST> m_expr;
public:
	IfAST(shared_ptr<ExprAST> cond, shared_ptr<ExprAST> expr)
		:m_cond(cond),
		m_expr(expr) {}
	~IfAST() {}
};