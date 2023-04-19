#include "ExprAST.h"
class ElseAST : public ExprAST {
	shared_ptr<ExprAST> m_expr;
public:
	ElseAST(shared_ptr<ExprAST> expr)
		:m_expr(expr) {}
	~ElseAST() {}
};