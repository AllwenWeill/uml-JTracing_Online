#include "allinclude.h"
#include "SyntaxKind.h"
#include "Token.h"
#include "Lexer.h"
class SyntaxNode{
public:
    SyntaxNode* parent = nullptr;
    SyntaxKind kind;

    SyntaxNode();
    ~SyntaxNode();
private:

};