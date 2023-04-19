#include "LogError.h"
LogError::LogError(){

}
LogError::~LogError(){
    
}
void LogError::addnote(string errorStr, int lineNum){//添加报错信息至errorNotes队列
    string errorNote = "Line number: ";
    errorNote += to_string(lineNum);
    errorNote += "-> Error: ";
    errorNote += errorStr;
    errorNote += ". \n";
    errorNotes.push_back(errorNote);
} 