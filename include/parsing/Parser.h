#include "allinclude.h"
#include "Token.h"
#include "TokenKind.h"
#include "LogError.h"
#include "LogParser.h"
#include "allAST.h"
struct VariableInformation {
    string name;
    string kind;
    string content = "";
};
struct FuncCallInformation {
    string invokeClassName;
    string FuncName;
    string callClassName; //爹
    int selfCall = 0;
    vector<int> loopCall;
    vector<int> childSequence;
    vector<int> alts_start;
    vector<int> else_start;
    vector<int> alts_end;
    vector<int> else_end;
};
class Parser {
public:
    Parser(vector<Token> tokenVector, vector<string> classNames);
    ~Parser();
    LogError LE;
    LogParser LogP;
    Token curToken;
    TokenKind curTokenKind;
    unordered_map<string, VariableInformation> VariableInfo_umap; //变量存储表
    unordered_set<TokenKind> Type_uset; //变量类型表
    unordered_map<int, FuncCallInformation> FuncCallInformation_umap;
    unordered_map<int, vector<string>> Loop_umap;
    unordered_map<int, vector<string>> Alt_umap;
    unordered_map<string, vector<int>> ClassActivation_umap;


private:
    VariableInformation VF; //该VF结构体需要不断被更新，生存周期直到Parser销毁
    unsigned long int m_offset;
    vector<string> m_classNames;
    TokenKind variableTypeFlag;
    unordered_map<string, int> BinopPrecedence_umap;
    vector<Token> m_tokenVector;
    unordered_map<string, vector<string>> ObjInstantiation_umap; //类实例化对应表：A-a1,a2; B-b1,b2;
    int m_classCounter; 
    std::shared_ptr<ExprAST> parsePrimary();
    std::shared_ptr<DefinitionAST> parseModule();
    std::shared_ptr<PrototypeAST> ParseModulePrototype();
    std::shared_ptr<DefinitionAST> ParseModuleDefinition();
    std::shared_ptr<ExprAST> ParseParenExpr();
    std::shared_ptr<ExprAST> ParseIdentifierExpr(TokenKind varType);
    std::shared_ptr<ExprAST> ParseExpression();
    std::shared_ptr<ExprAST> ParseNumber();
    std::shared_ptr<ExprAST> ParseBinOpRHS(int ExprPrec, std::shared_ptr<ExprAST> LHS);
    std::shared_ptr<ExprAST> ParseCmpOpRHS(std::shared_ptr<ExprAST> LHS);
    std::shared_ptr<ExprAST> ParseBegin();
    std::shared_ptr<ExprAST> ParseIf();
    std::shared_ptr<ExprAST> ParseElse();
    std::shared_ptr<ExprAST> ParseBitWide();
    std::shared_ptr<Always_ffAST> ParseAlways_ff();
    std::shared_ptr<Always_combAST> ParseAlways_comb();
    std::shared_ptr<InitialAST> ParseInitial();
    std::shared_ptr<ForAST> ParseFor();
    std::shared_ptr<ObjCallFuncPAST> ParseObjCallFuncP();
    std::shared_ptr<ObjCallFuncAAST> ParseObjCallFuncA();
    void mainParser();
    void getNextToken();
    int getClassCounter();
    void buildBinopPrecedence();
    void buildTypeUset();
    void buildObjInstantiationUmap();
    int GetTokPrecedence();
    void handlModule();
    void handlAlways_ff();
    void handlAlways_comb();
    void handInitial();
    void handlObj();
    void handInclude();
    void showErrorInformation();
    void showParserInformation();
    void showVariableInformation();
    string findClassName(string targetStr); //遍历ObjInstantiation_umap，通过a找其class名A
};