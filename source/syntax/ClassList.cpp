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

int ClassList::getClassCounter() {
	return ++m_classCounter;
}

ClassList::~ClassList() {
	cout << "Complete the build ClassList!" << endl;
}