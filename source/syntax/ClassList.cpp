//../../../test/output/FuncTable.txt
#include "ClassList.h"

ClassList::ClassList()
    :m_classCounter(0), m_altCounter(0), m_loopCounter(0)
{
    cout << "<Start building ClassList...>" << endl;
}

void ClassList::addFuncCallInfo(FuncCallInformation FC) {
    int  c = FuncCallInformation_umap.size();
    string a = FC.invokeClassName;
    FuncCallInformation_umap[getClassCounter()] = FC;
    string b = FuncCallInformation_umap.at(m_classCounter).invokeClassName;
    int  d = FuncCallInformation_umap.size();
}
void ClassList::modifyDescendantsSequence(int position, unordered_map<int, int> tmpCurDescendantsSequenceMap) {
    FuncCallInformation_umap[position].descendantsSequence = tmpCurDescendantsSequenceMap;
}
void ClassList::modifyDirectDescendantsSequence(int position, unordered_map<int, int> tmpCurDescendantsSequenceMap) {
    FuncCallInformation_umap[position].directDescendantsSequence = tmpCurDescendantsSequenceMap;
}
void ClassList::DeleteDirectDDescendantsSequenceEnum(int position, int key) {
    (FuncCallInformation_umap[position].directDescendantsSequence).erase(key);
}
void ClassList::setCallClassName(int callPosition, int beCallPosition) {
    FuncCallInformation_umap[callPosition].callClassName = FuncCallInformation_umap.at(beCallPosition).invokeClassName;
}
void ClassList::modifyClassActivationInfo(string className, int key, int value) {
    ClassActivation_umap[className][key] = value;
}

void ClassList::addLoopInfo(LoopInformation LP) {
    Loop_umap[getLoopCounter()] = LP;
}
void ClassList::addAltInfo(AltInformation AT) {
    //int p =Alt_umap.size();
    Alt_umap[getAltCounter()] = AT;
}

unordered_map<int, FuncCallInformation> ClassList::getFuncCallInfo() {
    return FuncCallInformation_umap;
}
unordered_map<string, unordered_map<int, int>> ClassList::getActivationInfo() {
    return ClassActivation_umap;
}
void ClassList::modifyAltInfo(AltInformation AT, int elsePosition, string elseCondition) {
        for (int i = 0; i < AT.altIncludeClassName.size(); i++) {
            Alt_umap[Alt_umap.size()].altIncludeClassName.push_back(AT.altIncludeClassName[i]);
        } for (int i = 0; i < AT.timeLine.size(); i++)
            Alt_umap[Alt_umap.size()].timeLine.push_back(AT.timeLine[i]);
        Alt_umap[Alt_umap.size()].elseTimeLine.push_back(elsePosition);
        if (elseCondition != "NULLCONDITION")
            Alt_umap[Alt_umap.size()].elseCondition.push_back(elseCondition);
}
unordered_map<int, LoopInformation> ClassList::getLoopInfo() {
    return Loop_umap;
}

unordered_map<int, AltInformation> ClassList::getAltInfo() {
    return Alt_umap;
}

int ClassList::getClassCounter() {
    return ++m_classCounter;
}
int ClassList::getAltCounter() {
    return ++m_altCounter;
}
int ClassList::getLoopCounter() {
    return ++m_loopCounter;
}
template<typename T>
void ClassList::outputVector(vector<T> vec) {
    for (auto v : vec) {
        umlFile << v << " ";
    }
}

template<typename T>
void ClassList::outputUmap(unordered_map<T, T> umap) {
    for (auto u : umap) {

    }
}

bool ClassList::writeUMLfile_FuncTable() {
    string startClassName = FuncCallInformation_umap.begin()->second.callClassName;
    startClassName = FuncCallInformation_umap.begin()->second.callClassName;
    for (int i = 1; i <= FuncCallInformation_umap.size(); i++) {
        if (FuncCallInformation_umap.at(i).invokeClassName == FuncCallInformation_umap.at(i).callClassName) {
            FuncCallInformation_umap[i].selfCall = 1;
        }
        int newActivation = 1;
        if (ClassActivation_umap.count(startClassName) > 0) {
            if (ClassActivation_umap.at(startClassName).count(i) > 0) {
                newActivation = ClassActivation_umap.at(startClassName).at(i) + 1;
            }
        }
        ClassActivation_umap[startClassName][i] = newActivation;
    }
    map outputMap = convertUnorderedMapToMap(FuncCallInformation_umap);
    m_FuncTablePath = "D:\\vs_workstation\\uml-JTracing_Online\\test\\output\\FuncTable.txt";
    umlFile.open(m_FuncTablePath);
    umlFile << "functionorder	invokeClassName		Funcname	callClassName	Selfcall" << endl;
    int count = 0;
    for (auto fc : outputMap) {
        umlFile << fc.first << "      " << fc.second.invokeClassName << "     " << fc.second.FuncName << "      " << fc.second.callClassName << "        " << fc.second.selfCall << endl;
    }
    umlFile.close();
    if (!fs::exists(m_FuncTablePath))
        return false;
    return true;
}

bool ClassList::writeUMLfile_LoopTable() {
    m_LoopTablePath = "D:\\vs_workstation\\uml-JTracing_Online\\test\\output\\LoopTable.txt";
    umlFile.open(m_LoopTablePath);
    umlFile << "looporder    loopclassname   		     timeline     Loopcondition" << endl;
    int count = 0;
    map outputMap = convertUnorderedMapToMap(Loop_umap);
    for (auto lc : outputMap) {
        umlFile << ++count << "          ";
        for (int i = 0; i < lc.second.loopIcludeClassName.size() - 1; i++) {
            umlFile << lc.second.loopIcludeClassName[i] << ",";
        }
        umlFile << lc.second.loopIcludeClassName[lc.second.loopIcludeClassName.size() - 1];
        umlFile << "     ";
        for (int i = 0; i < lc.second.timeLine.size() - 1; i++) {
            umlFile << lc.second.timeLine[i] << ",";
        }
        umlFile << lc.second.timeLine[lc.second.timeLine.size() - 1];
        umlFile << "   ";
        umlFile << lc.second.LoopCondition;
        umlFile << "   " << endl;
    }
    umlFile.close();
    if (!fs::exists(m_LoopTablePath))
        return false;
    return true;
}

bool ClassList::writeUMLfile_AltTable() {
    m_AltTablePath = "D:\\vs_workstation\\uml-JTracing_Online\\test\\output\\AltTable.txt";
    umlFile.open(m_AltTablePath);
    umlFile << "altorder    altclassname            timeline        elsetimeline     altCondition   elseCondintion" << endl;
    int count = 0;
    map outputMap = convertUnorderedMapToMap(Alt_umap);
    for (auto at : outputMap) {
        umlFile << at.first << "          ";
        if (at.second.altIncludeClassName.size() == 0)
            umlFile << "-1";
        else {
            vector<string> includeClass(at.second.altIncludeClassName);
            for (int i = 0; i < at.second.altIncludeClassName.size() - 1; i++) {
                umlFile << at.second.altIncludeClassName[i] << ",";
            }
            umlFile << at.second.altIncludeClassName[at.second.altIncludeClassName.size() - 1];
        }
        umlFile << "     ";
        if (at.second.timeLine.size() == 0)
            umlFile << "-1";
        else {
            for (int i = 0; i < at.second.timeLine.size() - 1; i++) {
                umlFile << at.second.timeLine[i] << ",";
            }
            umlFile << at.second.timeLine[at.second.timeLine.size() - 1];
        }
        umlFile << "     ";
        if (at.second.elseTimeLine.size() == 0)
            umlFile << "-1";
        else {
            for (int i = 0; i < at.second.elseTimeLine.size() - 1; i++) {
                umlFile << at.second.elseTimeLine[i] << ",";
            }
            umlFile << at.second.elseTimeLine[at.second.elseTimeLine.size() - 1];
        }

        umlFile << "   ";
        umlFile << at.second.altCondition;
        umlFile << "   ";
        if (at.second.elseCondition.size() == 0)
            umlFile << "-1";
        else {
            for (int i = 0; i < at.second.elseCondition.size() - 1; i++) {
                umlFile << at.second.elseCondition[i] << ",";
            }
            umlFile << at.second.elseCondition[at.second.elseCondition.size() - 1];
        }
        umlFile << "   " << endl;
    }

    umlFile.close();
    if (!fs::exists(m_AltTablePath))
        return false;
    return true;
}

bool ClassList::writeUMLfile_ActivationTable() {
    m_ActivationTablePath = "D:\\vs_workstation\\uml-JTracing_Online\\test\\output\\ActivationTable.txt";
    umlFile.open(m_ActivationTablePath);
    umlFile << "classname  activiationtime" << endl;

    for (auto fc : ClassActivation_umap) {
        umlFile << fc.first << "   ";
        int count = 1;
        for (auto fe : ClassActivation_umap.at(fc.first)) {
            if (count < ClassActivation_umap.at(fc.first).size()) {
                umlFile << fe.first << "." << fe.second;
                umlFile << ",";
                count++;
            }
            else
                umlFile << fe.first << "." << fe.second;
        }
        umlFile << "     " << endl;
    }
    umlFile.close();
    if (!fs::exists(m_ActivationTablePath))
        return false;
    return true;
}


template<typename KeyType, typename ValueType>
map<KeyType, ValueType> ClassList::convertUnorderedMapToMap(const std::unordered_map<KeyType, ValueType>& unorderedMap) {
    map<KeyType, ValueType> map(unorderedMap.begin(), unorderedMap.end());
    return map;
}

ClassList::~ClassList() {
    cout << "Complete the build ClassList!" << endl;
    if (writeUMLfile_FuncTable())
        std::cout << "build <FuncTable.txt> success!" << std::endl;
    else
        std::cout << "build <FuncTable.txt> failed!" << std::endl;
    if (writeUMLfile_LoopTable())
        std::cout << "build <LoopTable.txt> success!" << std::endl;
    else
        std::cout << "build <LoopTable.txt> failed!" << std::endl;
    if (writeUMLfile_AltTable())
        std::cout << "build <AltTable.txt> success!" << std::endl;
    else
        std::cout << "build <AltTable.txt> failed!" << std::endl;
    if (writeUMLfile_ActivationTable())
        std::cout << "build <ActivationTable.txt> success!" << std::endl;
    else
        std::cout << "build <ActivationTable.txt> failed!" << std::endl;
}
