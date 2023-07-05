#include "allinclude.h"
#include "Token.h"
#include "TokenKind.h"
#include "LogError.h"
#include "LogParser.h"
#include "allAST.h"
#include "ClassList.h"
struct VariableInformation {
    string name;
    string kind;
    string content = "";
};

class Parser {
public:
    Parser(unordered_map<string, vector<Token>> hTokenFlows, unordered_map<string, vector<Token>> cppTokenFlows, vector<Token> tokenVector, vector<string> classNames, ClassList *pCList, string curFileName, string startClassName, bool isSVfile);
    ~Parser();
    LogError LE;
    LogParser LogP;
    Token curToken;
    TokenKind curTokenKind;
    unordered_map<string, VariableInformation> VariableInfo_umap; //�����洢��
    unordered_set<TokenKind> Type_uset; //�������ͱ�
private:
    bool m_isSVfile;
    string m_startClassName;
    string m_curFileName; 
    unordered_map<string, vector<Token>> m_hTokenFlows;
    unordered_map<string, vector<Token>> m_cppTokenFlows;
    ClassList* m_pCList;
    vector<int>testvec;
    VariableInformation VF; //��VF�ṹ����Ҫ���ϱ����£���������ֱ��Parser����
    unsigned long int m_offset;
    vector<string> m_classNames;
    TokenKind variableTypeFlag;
    unordered_map<string, int> BinopPrecedence_umap;
    vector<Token> m_tokenVector;
    unordered_map<string, vector<string>> ObjInstantiation_umap; //��ʵ������Ӧ��A-a1,a2; B-b1,b2;
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
    std::shared_ptr<ExprAST> HandleAlt();
    std::shared_ptr<ExprAST> ParseIf();
    std::shared_ptr<ExprAST> ParseElse();
    std::shared_ptr<ExprAST> ParseBitWide();
    std::shared_ptr<Always_ffAST> ParseAlways_ff();
    std::shared_ptr<Always_combAST> ParseAlways_comb();
    std::shared_ptr<InitialAST> ParseInitial();
    std::shared_ptr<ForAST> ParseFor();
    std::shared_ptr<ObjCallFuncPAST> ParseObjCallFuncP();
    std::shared_ptr<ObjCallFuncAAST> ParseObjCallFuncA();
    std::shared_ptr<WhileAST> ParseWhile();
    std::shared_ptr<WhileAST> ParseDoWhile();
    void mainParser();
    void handlReturn();
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
    std::shared_ptr<FuncAST> handlObj();
    void handInclude();
    void handlFunc();
    void showErrorInformation();
    void showParserInformation();
    void showVariableInformation();
    string findClassName(string targetStr); //����ObjInstantiation_umap��ͨ��a����class��A
    vector<Token> filterTokenFlow(string targetFuncName, string targetfFleName);
    bool isClassName(string name);
    bool isFuncName(string targetStr);
};