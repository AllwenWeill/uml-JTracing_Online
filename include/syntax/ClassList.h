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
    unordered_map<int, int> descendantsSequence;
    unordered_map<int, int> directDescendantsSequence;
    int iterationCounter;
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
    void modifyDescendantsSequence(int, unordered_map<int, int>);
    void modifyDirectDescendantsSequence(int, unordered_map<int, int>);
    void DeleteDirectDDescendantsSequenceEnum(int, int key);
    void setCallClassName(int, int);
    void addFuncCallInfo(FuncCallInformation FC);
    void addLoopInfo(vector<string> LP);
    void addAltInfo();
    void addClassActivationInfo();
    unordered_map<int, FuncCallInformation> getFuncCallInfo();
    unordered_map<int, vector<string>> getLoopInfo();
    unordered_map<int, vector<string>> getAltInfo();
    unordered_map<string, vector<int>> getClassActivationInfo();
	ClassList();
	~ClassList();
};

#endif // !CLASSLIST_H
