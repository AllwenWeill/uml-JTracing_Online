// PrototypeAST - This class represents the "prototype" for a module,
// which captures its name, and its argument names (thus implicitly the number
// of arguments the function takes).
#include "ExprAST.h"
class PrototypeAST{
  std::string Name;
  std::vector<std::string> Args;

public:
  PrototypeAST(const std::string &name, std::vector<std::string> Args)
    : Name(name), Args(std::move(Args)) {}

  const std::string &getName() const { return Name; }
  ~PrototypeAST(){}
};