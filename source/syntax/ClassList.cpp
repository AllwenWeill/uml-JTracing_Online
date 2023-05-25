#include "ClassList.h"

ClassList::ClassList() 
	:m_classCounter(0)
{
	cout << "<Start building ClassList...>" << endl;
}

void ClassList::addFuncCallInfo(FuncCallInformation FC) {
	FuncCallInformation_umap[getClassCounter()] = FC;
}
void ClassList::addLoopInfo() {

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