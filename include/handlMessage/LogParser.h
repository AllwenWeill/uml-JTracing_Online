#ifndef _LOGPARSER_H
#define _LOGPARSER_H
#include "allinclude.h"
class LogParser {//����ר�Ÿ����������Ϣ��־
public:
    vector<string> parserNotes;
    LogParser();
    ~LogParser();
    void addnote(string parserStr); //��ӱ�����Ϣ��parserNotes����
private:
};
#endif