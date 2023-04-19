#include "ExprAST.h"
#include "NumberExprAST.h"
#include "VariableExprAST.h"
class BitWideAST : public ExprAST {
    string Op;
    std::shared_ptr<ExprAST> m_numWide, m_numRange;
    std::shared_ptr<ExprAST> m_BitWideName;

public:
    BitWideAST(std::shared_ptr<ExprAST> numWide,
        std::shared_ptr<ExprAST> numRange,
        std::shared_ptr<ExprAST> BitWideName)
        : Op(":"), m_numWide(numWide), m_numRange(numRange), m_BitWideName(BitWideName) {}
    ~BitWideAST() {}
};