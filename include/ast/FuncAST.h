// FuncAST
#include "ExprAST.h"
class FuncAST : public ExprAST {
    std::string m_funcName;
    //std::unique_ptr<ExprAST> Body;

public:
    FuncAST(std::string funcName)
        : m_funcName(funcName) {}
    ~FuncAST() {}
};