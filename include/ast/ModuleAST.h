// FunctionAST - This class represents a mould definition itself.
#include "ExprAST.h"
#include "PrototypeAST.h"
class FunctionAST {
  std::unique_ptr<PrototypeAST> Proto;
  std::unique_ptr<ExprAST> Body;

public:
  FunctionAST(std::unique_ptr<PrototypeAST> Proto,
              std::unique_ptr<ExprAST> Body)
    : Proto(std::move(Proto)), Body(std::move(Body)) {}
    ~FunctionAST(){}
};