#ifndef _TOKEN_H
#define _TOKEN_H

#include "allinclude.h"
#include "TokenKind.h"
struct TokenLocation{
    int m_tokenLine; //Token所在行号
    int m_tokenNum; //Token所存储的位置次序
};
class Token{
public:
    TokenLocation TL;
    Token(){};
    Token(TokenKind kind,int tokenNum, int tokenLine, string tokenString);
    ~Token();
    string getTokenKindStr(); //返回TokenKind的字符串，便于调试
    string getTokenStr(); //返回该Token的实际字符串
    TokenKind getTokenKind(); //返回TokenKind
private:
    string tokenString;
    TokenKind m_kind;
    string m_tokenString;
    string m_tokenkindStr;
};
string TokenKindtoString(TokenKind kind);
#endif