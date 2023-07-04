#ifndef CLASSLIST_H
#define CLASSLIST_H
#include "allinclude.h"
struct FuncCallInformation {
    string invokeClassName;
    string FuncName;
    string callClassName; 
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
struct AltInformation {
    vector<string> altIncludeClassName;
    vector<int> timeLine;
    vector<int> elseTimeLine;
    string  altCondition;
    vector<string>elseCondition;
};
struct LoopInformation {
    vector<string> loopIcludeClassName;
    vector<int> timeLine;
    string LoopCondition;
    vector<int>elseTimeline;
};
class ClassList {
private:
    template<typename KeyType, typename ValueType>
    map<KeyType, ValueType> convertUnorderedMapToMap(const unordered_map<KeyType, ValueType>& unorderedMap);
    std::ofstream umlFile;
    bool m_isSVfile;
    int m_classCounter;
    int m_altCounter;
    int m_loopCounter;
    fs::path m_FuncTablePath;
    fs::path m_LoopTablePath;
    fs::path m_AltTablePath;
    fs::path m_ActivationTablePath;
    unordered_map<int, FuncCallInformation> FuncCallInformation_umap;
    unordered_map<int, LoopInformation> Loop_umap;
    unordered_map<int, AltInformation> Alt_umap;
    unordered_map<string, unordered_map<int, int>> ClassActivation_umap;
    int getClassCounter();
    int getAltCounter();
    int getLoopCounter();
public:
    //int m_classCounter;
    void modifyDescendantsSequence(int position, unordered_map<int, int> tmpCurDescendantsSequenceMap);
    void modifyDirectDescendantsSequence(int position, unordered_map<int, int> tmpCurDescendantsSequenceMap);
    void DeleteDirectDDescendantsSequenceEnum(int position, int key);
    void setCallClassName(int callPosition, int beCallPosition);
    void addFuncCallInfo(FuncCallInformation FC);
    void addLoopInfo(LoopInformation LP);
    void addAltInfo(AltInformation AT);
    void addClassActivationInfo();
    template<typename T>
    void outputVector(vector<T> vec);
    template<typename T>
    void outputUmap(unordered_map<T, T> umap);
    void modifyAltInfo(AltInformation AT, int elsePosition, string elseCondition);
    void modifyClassActivationInfo(string className, int key, int value);
    bool writeUMLfile_FuncTable();
    bool writeUMLfile_LoopTable();
    bool writeUMLfile_AltTable();
    bool writeUMLfile_ActivationTable();
    unordered_map<int, FuncCallInformation> getFuncCallInfo();
    unordered_map<string, unordered_map<int, int>> getActivationInfo();
    unordered_map<int, LoopInformation> getLoopInfo();
    unordered_map<int, AltInformation> getAltInfo();
    unordered_map<string, vector<int>> getClassActivationInfo();
    ClassList(bool isSVfile);
    ~ClassList();
};
#endif // !CLASSLIST_H