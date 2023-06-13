#ifndef _LOGPARSER_H
#define _LOGPARSER_H
#include "allinclude.h"
class LogParser {//该类专门负责处理解析信息日志
public:
    vector<string> parserNotes;
    LogParser();
    ~LogParser();
    void addnote(string parserStr); //添加报错信息至parserNotes容器
private:
};
#endif