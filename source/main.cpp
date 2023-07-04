//#include "SourceManager.h"
#include "Lexer.h"
#include "LogError.h"
#include "Parser.h"
#include "allinclude.h"
#include "ClassList.h"

int main(int argc, char* argv[]){
    fs::path fp;
    SourceManager SM(fp);
    ClassList CList;
    ClassList* pCList = &CList;
    unordered_map<string, vector<Token>> hTokenFlows;
    unordered_map<string, vector<Token>> cppTokenFlows;
    vector<string> classNames;
    bool isSVfile = false;
    if (!SM.isExitSVfile()) { //cpp
        for (auto hFile : SM.gethFiles()) {
            Lexer lex(&hFile.second, hFile.second.size());
            cout << hFile.first << endl;
            hTokenFlows[hFile.first] = lex.getTokenVector();
            if (lex.getClassName() != "") {
                classNames.emplace_back(lex.getClassName());
            }
        }
        cout << "----------------------------------------------------" << endl;
        for (auto cppFile : SM.getcppFiles()) {
            Lexer lex(&cppFile.second, cppFile.second.size());
            cppTokenFlows[cppFile.first] = lex.getTokenVector();
        }
        cout << "----------------------------------------------------" << endl;
        cout << "classNames: ";
        for (auto cn : classNames) {
            cout << cn << ",";
        }
        cout << endl;
        cout << "----------------------------------------------------" << endl;
        Parser par(hTokenFlows, cppTokenFlows, cppTokenFlows["main.cpp"], classNames, pCList, "main.cpp", "MainClass", false);
    }
    else { //sv
        string* psm = &SM.fd.filememo;
        Lexer lex(psm, SM.fd.filesize);
        Parser par(hTokenFlows, cppTokenFlows, lex.getTokenVector(), classNames, pCList, "main.cpp", "MainClass", true);
        isSVfile = true;
    }
    if (!isSVfile) { //normal cpp 
        std::string exePath = "D:\\vs_workstation\\uml-JTracing_Online\\test\\output\\umlDrawing.exe";
        std::string command = exePath;

        // 获取可执行文件所在目录的路径
        size_t pos = exePath.find_last_of("\\/");
        std::string exeDir = exePath.substr(0, pos);

        // 改变当前进程的工作目录为可执行文件所在目录
        if (!SetCurrentDirectoryA(exeDir.c_str())) {
            std::cout << "Failed to change working directory." << std::endl;
            return 1;
        }
        int res = system(exePath.c_str());
        if (res == 0) {
            cout << "Drawing UML sequence diagram..." << endl;
        }
        else if (res != 0)
            cout << "Error: UML build failed!" << endl;
    }
    return 0;
}
