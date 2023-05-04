//#include "SourceManager.h"
#include "Lexer.h"
#include "LogError.h"
#include "Parser.h"

int main(int argc, char* argv[]){
    //cout<<"输入.sv文件路径:";
    fs::path fp;
    //cin>>fp;
    fp = "../../../test/test_module.sv";
    SourceManager SM(fp);
    //string text = "int ;";
    //cin >> text;
    //SourceManager SM(text);
    string *psm = &SM.fd.filememo;
    cout<<*psm<<endl;
    cout<<"------------"<<endl;
    Lexer lex(psm, SM.fd.filesize);
    cout<<endl;
    Parser par(lex.getTokenVector());
    return 0;
}
