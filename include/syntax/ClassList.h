#ifndef CLASSLIST_H
#define CLASSLIST_H
#include "allinclude.h"
struct FuncCallInformation {
    string invokeClassName;
    string FuncName;
    string callClassName; //调用该成员函数的类
    int selfCall = 0;
    vector<int> loopCall;
    vector<int> childSequence;
    vector<int> alts_start;
    vector<int> else_start;
    vector<int> alts_end;
    vector<int> else_end;
};
class ClassList {
private:
    int m_classCounter;
    unordered_map<int, FuncCallInformation> FuncCallInformation_umap;
    unordered_map<int, vector<string>> Loop_umap;
    unordered_map<int, vector<string>> Alt_umap;
    unordered_map<string, vector<int>> ClassActivation_umap;
    int getClassCounter();
public:
    void addFuncCallInfo(FuncCallInformation FC);
    void addLoopInfo();
    void addAltInfo();
    void addClassActivationInfo();
	ClassList();
	~ClassList();
};

#endif // !CLASSLIST_H
