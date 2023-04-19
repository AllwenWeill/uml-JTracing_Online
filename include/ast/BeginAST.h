#include "ExprAST.h"
class BeginAST : public ExprAST{
	vector<shared_ptr<ExprAST>> Exprs;
public:
	BeginAST(vector<shared_ptr<ExprAST>> exprs)
		:Exprs(exprs) {}
	~BeginAST() {}

};