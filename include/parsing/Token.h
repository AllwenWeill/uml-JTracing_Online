#ifndef _TOKEN_H
#define _TOKEN_H

#include "allinclude.h"
#include "TokenKind.h"
struct TokenLocation{
    int m_tokenLine; //Token�����к�
    int m_tokenNum; //Token���洢��λ�ô���
};

class Token{
public:
    TokenLocation TL;
    Token(){};
    Token(TokenKind kind,int tokenNum, int tokenLine, string tokenString);
    ~Token();
    string getTokenKindStr(); //����TokenKind���ַ��������ڵ���
    string getTokenStr(); //���ظ�Token��ʵ���ַ���
    TokenKind getTokenKind(); //����TokenKind

private:
    string tokenString;
    TokenKind m_kind;
    string m_tokenString;
    string m_tokenkindStr;
};
string TokenKindtoString(TokenKind kind);
#endif