#ifndef _LOGERROR_H
#define _LOGERROR_H
#include "allinclude.h"
struct errorInformation{ //报错信息结构体
    int lineNum;
    string errorStr;
};
class LogError{//该类专门负责处理错误信息
public:
    vector<string> errorNotes;
    LogError();
    ~LogError();    
    void addnote(string errorStr, int lineNum); //添加报错信息至errorNotes队列
private:
};
#endif