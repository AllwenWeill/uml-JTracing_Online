#ifndef _VARIABLEEXPRAST_H
#define _VARIABLEEXPRAST_H
#include "ExprAST.h"
class VariableExprAST : public ExprAST {
    std::string m_varName;
    std::string m_varType;
public:
    VariableExprAST(const std::string Name, const std::string Type) 
        : m_varName(Name),
        m_varType(Type){}
    ~VariableExprAST(){}
};
#endif