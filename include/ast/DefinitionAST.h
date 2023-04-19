#include "ExprAST.h"
class DefinitionAST{
    string moduleName;
    vector<shared_ptr<ExprAST>> Exprs;
public:
    DefinitionAST(string name, vector<shared_ptr<ExprAST>> exprs)
    : moduleName(name),
    Exprs(exprs){}
    ~DefinitionAST(){}
};
