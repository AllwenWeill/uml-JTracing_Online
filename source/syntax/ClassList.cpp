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

bool ClassList::writeUMLfile_FuncTable() {
	m_FuncTablePath = "../../../test/output/FuncTable.txt";
	umlFile.open(m_FuncTablePath, 0x02);
	umlFile << "functionorder	invokeClassName		Funcname	callClassName	Selfcall" << endl;
	int count = 0;
	for(auto fc : FuncCallInformation_umap){
		umlFile << ++count << "      " << fc.second.invokeClassName << "     " << fc.second.FuncName << "      " << fc.second.callClassName << "        " << fc.second.selfCall << endl;
	}
	umlFile.close();
	if (!fs::exists(m_FuncTablePath))
		return false;
	return true;
}

bool ClassList::writeUMLfile_LoopTable() {
	m_LoopTablePath = "../../../test/output/LoopTable.txt";
	umlFile.open(m_LoopTablePath, 0x02);
	umlFile << "looporder    loopclassname   		      timeline" << endl;
	int count = 0;
	for (auto lc : Loop_umap) {
		umlFile << ++count << endl; // undo
	}
	umlFile.close();
	if (!fs::exists(m_LoopTablePath))
		return false;
	return true;
}

bool ClassList::writeUMLfile_AltTable() {
	m_AltTablePath = "../../../test/output/AltTable.txt";
	umlFile.open(m_AltTablePath, 0x02);
	umlFile << "altorder    altclassname            timeline        elsetimeline" << endl;
	int count = 0;
	for (auto au : Alt_umap) {
		umlFile << ++count << endl; // undo
	}
	umlFile.close();
	if (!fs::exists(m_AltTablePath))
		return false;
	return true;
}

bool ClassList::writeUMLfile_ActivationTable() {
	m_ActivationTablePath = "../../../test/output/ActivationTable.txt";
	umlFile.open(m_ActivationTablePath, 0x02);
	umlFile << "classname  activiationtime" << endl;
	int count = 0;
									// undo
	umlFile.close();
	if (!fs::exists(m_ActivationTablePath))
		return false;
	return true;
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