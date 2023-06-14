#include "ClassList.h"

ClassList::ClassList() 
	:m_classCounter(0)
{
	cout << "<Start building ClassList...>" << endl;
}

void ClassList::addFuncCallInfo(FuncCallInformation FC) {
	FuncCallInformation_umap[getClassCounter()] = FC;
}

void ClassList::modifyDescendantsSequence(int position, unordered_map<int, int> tmpCurDescendantsSequenceMap) {
	FuncCallInformation_umap.at(position).descendantsSequence = tmpCurDescendantsSequenceMap;
}

void ClassList::modifyDirectDescendantsSequence(int position, unordered_map<int, int> tmpCurDescendantsSequenceMap) {
	FuncCallInformation_umap.at(position).directDescendantsSequence = tmpCurDescendantsSequenceMap;
}

void ClassList::DeleteDirectDDescendantsSequenceEnum(int position, int key) {
	FuncCallInformation_umap.at(position).directDescendantsSequence.erase(key);
}

void ClassList::setCallClassName(int callPosition, int beCallPosition) {
	FuncCallInformation_umap.at(callPosition).callClassName = FuncCallInformation_umap.at(beCallPosition).invokeClassName;
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

bool ClassList::writeUMLfile() {
	umlFile.open(m_builtUMLPath, 0x02);
	umlFile << "functionorder	invokeClassName		Funcname	callClassName	Selfcall	loopcall	childSequence	alts_start	else_start	 alts_end	else_end" << endl;
	int count = 0;
	for(auto fc : FuncCallInformation_umap){
		umlFile << ++count << "      " << fc.second.invokeClassName << "     " << fc.second.FuncName << "      " << fc.second.callClassName << "        " << fc.second.selfCall << "        " ;
		outputVector(fc.second.loopCall);
		umlFile << "      ";
		outputVector(fc.second.childSequence);
		umlFile << "      ";
		outputVector(fc.second.alts_start);
		umlFile << "      ";
		outputVector(fc.second.else_start);
		umlFile << "      ";
		outputVector(fc.second.alts_end);
		umlFile << "      ";
		outputVector(fc.second.else_end);
		umlFile << "      ";

	}
	if (!fs::exists(m_builtUMLPath))
		return false;
	return true;
}

ClassList::~ClassList() {
	cout << "Complete the build ClassList!" << endl;
	m_builtUMLPath = "../../../test/output/builtUML.txt";
	if (writeUMLfile())
		std::cout << "build <builtUML.txt> success!" << std::endl;
	else
		std::cout << "build <builtUML.txt> failed!" << std::endl;

}