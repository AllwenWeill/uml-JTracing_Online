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
};
struct LoopInformation {
    vector<string> loopIcludeClassName;
    vector<int> timeLine;
    vector<int>elseTimeline;
};
class ClassList {
private:
    std::ofstream umlFile;
    int m_classCounter;
    fs::path m_FuncTablePath;
    fs::path m_LoopTablePath;
    fs::path m_AltTablePath;
    fs::path m_ActivationTablePath;

    unordered_map<int, LoopInformation> Loop_umap;
    unordered_map<int, AltInformation> Alt_umap;
    unordered_map<string, unordered_map<int, int>> ClassActivation_umap;
    int getClassCounter();
public:
    unordered_map<int, FuncCallInformation> FuncCallInformation_umap;
    void modifyDescendantsSequence(int, unordered_map<int, int>);
    void modifyDirectDescendantsSequence(int, unordered_map<int, int>);
    void DeleteDirectDDescendantsSequenceEnum(int, int);
    void setCallClassName(int, int);
    void addFuncCallInfo(FuncCallInformation);
    void addLoopInfo(LoopInformation);
    void addAltInfo(AltInformation);
    void addClassActivationInfo();

    template<typename T>
    void outputVector(vector<T> vec);

    template<typename T>
    void outputUmap(unordered_map<T, T> umap);
    void modifyAltInfo(AltInformation, int);
    void modifyClassActivationInfo(string, int, int);
    bool writeUMLfile_FuncTable();
    bool writeUMLfile_LoopTable();
    bool writeUMLfile_AltTable();
    bool writeUMLfile_ActivationTable();
    unordered_map<int, FuncCallInformation> getFuncCallInfo();
    unordered_map<string, unordered_map<int, int>> getActivationInfo();
    unordered_map<int, LoopInformation> getLoopInfo();
    unordered_map<int, AltInformation> getAltInfo();
    unordered_map<string, vector<int>> getClassActivationInfo();
    ClassList();
    ~ClassList();
};

#endif // !CLASSLIST_H