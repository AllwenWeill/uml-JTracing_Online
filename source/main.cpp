//#include "SourceManager.h"
#include "Lexer.h"
#include "LogError.h"
#include "Parser.h"
#include "allinclude.h"
#include "ClassList.h"

int main(int argc, char* argv[]){
    fs::path fp;
    SourceManager SM(fp);
    unordered_map<string, vector<Token>> hTokenFlows;
    unordered_map<string, vector<Token>> cppTokenFlows;
    vector<string> classNames;
    bool isSVfile = false;
    if (!SM.isExitSVfile()) { //cpp
        ClassList CList(false);
        ClassList* pCList = &CList;
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
        ClassList CList(true);
        ClassList* pCList = &CList;
        string* psm = &SM.fd.filememo;
        Lexer lex(psm, SM.fd.filesize);
        Parser par(hTokenFlows, cppTokenFlows, lex.getTokenVector(), classNames, pCList, "main.cpp", "MainClass", true);
        isSVfile = true;
    }
        return 0;
}
