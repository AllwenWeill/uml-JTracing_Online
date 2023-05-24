//#include "SourceManager.h"
#include "Lexer.h"
#include "LogError.h"
#include "Parser.h"
#include "allinclude.h"
#include "ClassList.h"

int main(int argc, char* argv[]){
    //cout<<"输入.sv文件路径:";
    fs::path fp;
    //cin>>fp;
    fp = "../../../test/test_module.sv";
    SourceManager SM(fp);
    //string text = "int ;";
    //cin >> text;
    vector<vector<Token>> hTokenFlows;
    vector<vector<Token>> cppTokenFlows;
    vector<string> classNames;
    for (auto hFile : SM.gethFiles()) {
        Lexer lex(&hFile.second, hFile.second.size());
        hTokenFlows.emplace_back(lex.getTokenVector());
        if (lex.getClassName() != "") {
            classNames.emplace_back(lex.getClassName());
        }
    }
    cout << "----------------------------------------------------" << endl;
    for (auto cppFile : SM.getcppFiles()) {
        Lexer lex(&cppFile.second, cppFile.second.size());
        cppTokenFlows.emplace_back(lex.getTokenVector());
    }
    cout << "----------------------------------------------------" << endl;
    cout << "classNames: ";
    for (auto cn : classNames) {
        cout << cn << ",";
    }
    cout << endl;
    cout << "----------------------------------------------------" << endl;
    ClassList CList;
    ClassList* pCList = &CList;
    Parser par(cppTokenFlows[0], classNames, pCList);
    //SourceManager SM(text);
    /*string* psm = &SM.fd.filememo;
    Lexer lex(psm, SM.fd.filesize);
    cout<<endl;
    Parser par(lex.getTokenVector());*/
    return 0;
}
