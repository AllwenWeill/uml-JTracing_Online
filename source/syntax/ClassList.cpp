#include "ClassList.h"

ClassList::ClassList() 
	:m_classCounter(0)
{
	cout << "<Start building ClassList...>" << endl;
}

void ClassList::addFuncCallInfo(FuncCallInformation FC) {
	FuncCallInformation_umap[getClassCounter()] = FC;
}
void ClassList::modifyDescendantsSequence(int position, unordered_map<int,int> tmpCurDescendantsSequenceMap){
    FuncCallInformation_umap.at(position).descendantsSequence = tmpCurDescendantsSequenceMap;
}
void ClassList::modifyDirectDescendantsSequence(int position, unordered_map<int,int> tmpCurDescendantsSequenceMap) {
    FuncCallInformation_umap.at(position).directDescendantsSequence = tmpCurDescendantsSequenceMap;
}
void ClassList::DeleteDirectDDescendantsSequenceEnum(int position, int key){
    FuncCallInformation_umap.at(position).directDescendantsSequence.erase(key);
}
void ClassList::setCallClassName(int callPosition, int beCallPosition) {
    FuncCallInformation_umap.at(callPosition).callClassName =FuncCallInformation_umap.at(beCallPosition).invokeClassName;
  }

void ClassList::addLoopInfo(vector<string> LP) {
    Loop_umap[getClassCounter()] = LP;
}
void ClassList::addAltInfo() {

}
void ClassList::addClassActivationInfo() {

}

unordered_map<int, FuncCallInformation> ClassList::getFuncCallInfo() {
	return FuncCallInformation_umap;
}

unordered_map<int, vector<string>> ClassList::getLoopInfo() {
	return Loop_umap;
}

unordered_map<int, vector<string>> ClassList::getAltInfo() {
	return Alt_umap;
}

unordered_map<string, vector<int>> ClassList::getClassActivationInfo() {
	return ClassActivation_umap;
}

int ClassList::getClassCounter() {
	return ++m_classCounter;
}

ClassList::~ClassList() {
	cout << "Complete the build ClassList!" << endl;
}