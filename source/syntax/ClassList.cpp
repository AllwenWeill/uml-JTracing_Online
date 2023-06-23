#include "ClassList.h"

ClassList::ClassList() 
	:m_classCounter(0)
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

void ClassList::modifyClassActivationInfo(string className, int key, int value) {
	ClassActivation_umap[className][key] = value;
}

void ClassList::addLoopInfo(LoopInformation LP) {
	Loop_umap[getClassCounter()] = LP;
}

void ClassList::addAltInfo(AltInformation AT) {
	Alt_umap[getClassCounter()] = AT;
}

void ClassList::addClassActivationInfo() {

}

unordered_map<int, FuncCallInformation> ClassList::getFuncCallInfo() {
	return FuncCallInformation_umap;
}

unordered_map<string, unordered_map<int, int>> ClassList::getActivationInfo() {
	return ClassActivation_umap;
}

void ClassList::modifyAltInfo(AltInformation AT, int elsePosition) {
	for (int i = 0; i < AT.altIncludeClassName.size(); i++)
		Alt_umap[Alt_umap.size()].altIncludeClassName.push_back(AT.altIncludeClassName[i]);
	Alt_umap[Alt_umap.size()].elseTimeLine.push_back(elsePosition);
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
		umlFile << ++count << "          ";
		for (int i = 0; i < lc.second.loopIcludeClassName.size(); i++)
			umlFile << lc.second.loopIcludeClassName[i] << ",";
		umlFile << "     ";
		for (int i = 0; i < lc.second.timeLine.size(); i++)
			umlFile << lc.second.timeLine[i] << ",";
		umlFile << "   " << endl;
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
	for (auto at : Alt_umap) {
		umlFile << ++count << "          ";
		for (int i = 0; i < at.second.altIncludeClassName.size(); i++)
			umlFile << at.second.altIncludeClassName[i] << ",";
		umlFile << "     ";
		for (int i = 0; i < at.second.timeLine.size(); i++)
			umlFile << at.second.timeLine[i] << ",";
		umlFile << "     ";
		for (int i = 0; i < at.second.timeLine.size(); i++)
			umlFile << at.second.elseTimeLine[i] << ",";
		umlFile << "   " << endl;
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
	for (auto fc : ClassActivation_umap) {
		umlFile << ++count << "      " << fc.first << "   ";
		for (auto fe : ClassActivation_umap.at(fc.first)) {
			umlFile << fe.first << "." << fe.second;
			umlFile << ", ";
		}
		umlFile << "     " << endl;
	}
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