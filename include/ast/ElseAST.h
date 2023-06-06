#include "ExprAST.h"
class ElseAST : public ExprAST {
	vector<shared_ptr<ExprAST>> m_exprs;
public:
	ElseAST(vector<shared_ptr<ExprAST>> expr)
		:m_exprs(expr) {}
	~ElseAST() {}
};