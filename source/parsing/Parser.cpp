#include "Parser.h"
Parser::Parser(unordered_map<string, vector<Token>> hTokenFlows, unordered_map<string, vector<Token>> cppTokenFlows, vector<Token> tokenVector, vector<string> classNames, ClassList* pCList)
    :m_hTokenFlows(hTokenFlows),
    m_cppTokenFlows(cppTokenFlows),
    m_tokenVector(tokenVector),
    variableTypeFlag(TokenKind::NullKeyword),
    m_classNames(classNames),
    m_pCList(pCList)
{
    m_offset = 0;
    buildTypeUset();
    buildBinopPrecedence();
    buildObjInstantiationUmap();
    if (m_tokenVector.size() != 0) {
        curToken = m_tokenVector[0];
        curTokenKind = curToken.getTokenKind();
    }
    mainParser();
}

Parser::~Parser() {
    showParserInformation();
    showErrorInformation();
    showVariableInformation();
}

void Parser::mainParser() {
    while(m_offset < m_tokenVector.size() - 1) {
        //cout << "ready> "<<endl;
        switch (curTokenKind) {
        case TokenKind::ModuleKeyword:
            handlModule();
            break;
        case TokenKind::EndOfFile:
            return;
        case TokenKind::Semicolon:
            getNextToken();
            break;
        case TokenKind::AlwaysFFKeyword:
            handlAlways_ff();
            break;
        case TokenKind::AlwaysCombKeyword:
            handlAlways_comb();
            break;
        case TokenKind::InitialKeyword:
            handInitial();
            break;
        case TokenKind::Identifier:
            handlObj();
            break;
        case TokenKind::IncludeKeyword:
            handInclude();
            break;
        default:
            if (Type_uset.count(curTokenKind)) { //�����Type���У���˵����ǰtokenΪint�����͹ؼ���
                getNextToken(); //eat type
                handlFunc();
                
            }
            ParseExpression();
            break;
        }
    }
}

void Parser::getNextToken() {
    curToken = m_tokenVector[m_offset++];
    curTokenKind = curToken.getTokenKind();
}

std::shared_ptr<ExprAST> Parser::parsePrimary() { //�����������ʽ
    switch (curTokenKind) {
    case TokenKind::Unknown: {
        string tmpStr = "Unknown the ";
        tmpStr += curToken.getTokenStr();
        LE.addnote(tmpStr, curToken.TL.m_tokenLine);
        return nullptr;
    }
    case TokenKind::IntKeyword:
        LogP.addnote("->parsing a IntVariable...");
        variableTypeFlag = TokenKind::IntKeyword;
        break;
    case TokenKind::RegKeyword:
        LogP.addnote("->parsing a RegVariable...");
        variableTypeFlag = TokenKind::RegKeyword;
        break;
    case TokenKind::BitKeyword:
        LogP.addnote("->parsing a BitVariable...");
        variableTypeFlag = TokenKind::BitKeyword;
        break;
    case TokenKind::ByteKeyword:
        LogP.addnote("->parsing a ByteVariable...");
        variableTypeFlag = TokenKind::ByteKeyword;
        break;
    case TokenKind::ShortIntKeyword:
        LogP.addnote("->parsing a ShortIntVariable...");
        variableTypeFlag = TokenKind::ShortIntKeyword;
        break;
    case TokenKind::LongIntKeyword:
        LogP.addnote("->parsing a LongIntVariable...");
        variableTypeFlag = TokenKind::LongIntKeyword;
        break;
    case TokenKind::IntegerKeyword:
        LogP.addnote("->parsing a IntegerVariable...");
        variableTypeFlag = TokenKind::IntegerKeyword;
        break;
    case TokenKind::LogicKeyword:
        LogP.addnote("->parsing a LogicVariable...");
        variableTypeFlag = TokenKind::LogicKeyword;
        break;
    case TokenKind::PosEdgeKeyword:
        LogP.addnote("->parsing a PosEdgeVariable...");
        variableTypeFlag = TokenKind::PosEdgeKeyword;
        break;
    case TokenKind::BeginKeyword: {
        auto V = ParseBegin();
        return std::move(V);
    }
    case TokenKind::IfKeyword:
    {
        auto V = ParseIf();
        return std::move(V);
    }
    case TokenKind::ElseKeyword:
    {
        auto V = ParseElse();
        return std::move(V);
    }
    case TokenKind::AlwaysFFKeyword: {
        auto V = ParseAlways_ff();
        return std::move(V);
    }   
    case TokenKind::AlwaysCombKeyword: {
        auto V = ParseAlways_comb();
        return std::move(V);
    }
    case TokenKind::ForKeyword: {
        LogP.addnote("parsing for:");
        auto V = ParseFor();
        return std::move(V);
    }
    case TokenKind::Identifier:
        return ParseIdentifierExpr(TokenKind::NullKeyword);
    case TokenKind::DoublePlus:
    case TokenKind::DoubleMinus: {
        auto V = std::make_shared<NumberExprAST>(1);
        getNextToken();//eat op
        return std::move(V);
    }
    }
    getNextToken();
    switch (curTokenKind) {
    case TokenKind::Identifier:
        return ParseIdentifierExpr(variableTypeFlag);
    case TokenKind::IntegerLiteral:
        return ParseNumber();
    case TokenKind::OpenBracket:
        return ParseBitWide();
    case TokenKind::Unknown: {
        string tmpStr = "Unknown the ";
        tmpStr += curToken.getTokenStr();
        LE.addnote(tmpStr, curToken.TL.m_tokenLine);
        return nullptr;
    }
    case TokenKind::OpenParenthesis:
        return ParseParenExpr();
    case TokenKind::Semicolon:
        LE.addnote("expected expression", curToken.TL.m_tokenLine);
        return nullptr;
    default:
        LE.addnote("expcted ';'", curToken.TL.m_tokenLine);
        return nullptr;
    }
}

/*std::shared_ptr<ObjCallFuncPAST> Parser::ParseObjCallFuncP() {

}

std::shared_ptr<ObjCallFuncAAST> Parser::ParseObjCallFuncA() {

}
*/
std::shared_ptr<ExprAST> Parser::ParseBitWide() {
    getNextToken();
    if (curTokenKind != TokenKind::IntegerLiteral) { //[]�ڵ�һ��Token�ڴ������������͵ĳ���
        LE.addnote("reference to non-constant variable is not allowed in a constant expression", curToken.TL.m_tokenLine);
        return nullptr;
    }
    auto numWide = ParseNumber(); 
    if (curTokenKind != TokenKind::Colon) { //��ʱ�ڴ�һ��:
        LE.addnote("packed dimensions require a full range specification", curToken.TL.m_tokenLine);
        return nullptr;
    }
    getNextToken();
    if (curTokenKind != TokenKind::IntegerLiteral) { //[]�����һ��Token�ڴ������������͵ĳ���
        LE.addnote("reference to non-constant variable is not allowed in a constant expression", curToken.TL.m_tokenLine);
        return nullptr;
    }
    auto numRange = ParseNumber();
    if (curTokenKind != TokenKind::CloseBracket) { //��ʱ�ڴ�һ��]
        LE.addnote("expected ']'", curToken.TL.m_tokenLine);
        return nullptr;
    }
    getNextToken();
    auto BitWideName = ParseIdentifierExpr(TokenKind::BitKeyword);
    auto res = std::make_shared<BitWideAST>(numWide, numRange, BitWideName);
    return std::move(res);
}

std::shared_ptr<ExprAST> Parser::ParseNumber() {
    string s = curToken.getTokenStr();
    int numVal = atoi(s.c_str());
    auto Result = std::make_shared<NumberExprAST>(numVal);
    getNextToken(); // consume the number
    return std::move(Result);
}

// std::shared_ptr<PrototypeAST> Parser::ParseModulePrototype(){ //����module���������ܲ���Ҫ����
//     getNextToken();
//     if(curTokenKind != TokenKind::Identifier){ //module��Ҫ��module��
//         LE.addnote("expected function name in module", curToken.TL.m_tokenLine);
//         return nullptr;
//     }
//     std::string moduleName = curToken.getTokenStr();
//     getNextToken();
//     if(curTokenKind != TokenKind::Semicolon){
//         LE.addnote("expected ';'", curToken.TL.m_tokenLine);
//         return nullptr;
//     }
//     return std::make_shared<PrototypeAST>(moduleName);
// }

std::shared_ptr<DefinitionAST> Parser::ParseModuleDefinition() { //����moduleʵ��
    getNextToken();
    if (curTokenKind != TokenKind::Identifier) { //module��Ҫ��module��
        LE.addnote("expected function name in module", curToken.TL.m_tokenLine);
        return nullptr;
    }
    std::string moduleName = curToken.getTokenStr();
    getNextToken();
    if (curTokenKind != TokenKind::Semicolon) {
        LE.addnote("expected ';'", curToken.TL.m_tokenLine);
        //return nullptr;
    }
    std::vector<shared_ptr<ExprAST>> Exprs;
    while (curTokenKind != TokenKind::EndModuleKeyword) {
        getNextToken();
        if (curTokenKind == TokenKind::EndModuleKeyword)
            break;
        if (m_offset == m_tokenVector.size() - 1 && curTokenKind != TokenKind::EndModuleKeyword) {
            LE.addnote("expected 'endmodule'", curToken.TL.m_tokenLine);
            break;
        }
        auto V = ParseExpression();
        if (V != nullptr)
            Exprs.push_back(V);
        else
            m_offset--;
    }
    return std::make_shared<DefinitionAST>(moduleName, Exprs);
}

std::shared_ptr<Always_ffAST> Parser::ParseAlways_ff() {
    getNextToken(); //eat Always_ff�ؼ���
    if (curTokenKind != TokenKind::At) {
        LE.addnote("always_ff procedure must have one and only one event control", curToken.TL.m_tokenLine);
        return nullptr;
    }
    getNextToken(); //eat @
    if (curTokenKind != TokenKind::OpenParenthesis) {
        LE.addnote("always_ff procedure must have one and only one event control", curToken.TL.m_tokenLine);
        return nullptr;
    }
    auto ff_event = ParseParenExpr();
    if (ff_event == nullptr) {
        LE.addnote("expected statement", curToken.TL.m_tokenLine);
        return nullptr;
    }
    auto exprs = parsePrimary();
    return std::make_shared<Always_ffAST>(ff_event, exprs);
}

std::shared_ptr<Always_combAST> Parser::ParseAlways_comb() {
    getNextToken(); //eat Always_comb�ؼ���
    auto exprs = parsePrimary();
    return std::make_shared<Always_combAST>(exprs);
}

std::shared_ptr<ForAST> Parser::ParseFor() {
    getNextToken(); //eat for�ؼ���
    if (curTokenKind != TokenKind::OpenParenthesis) { //��ʱ�ڴ�һ��(
        LE.addnote("expected '('", curToken.TL.m_tokenLine);
        return nullptr;
    }
    getNextToken(); //eat (
    auto init = ParseExpression();
    if (curTokenKind != TokenKind::Semicolon) {
        LE.addnote("expected ';'", curToken.TL.m_tokenLine);
    }
    getNextToken(); //eat ;
    auto LHS = ParseIdentifierExpr(TokenKind::NullKeyword);
    auto cmp = ParseCmpOpRHS(LHS);
    if (curTokenKind != TokenKind::Semicolon) {
        LE.addnote("expected ';'", curToken.TL.m_tokenLine);
    }
    getNextToken(); //eat ;
    auto step = ParseExpression();
    if (curTokenKind != TokenKind::CloseParenthesis) {
        LE.addnote("expected ')'", curToken.TL.m_tokenLine);
        return nullptr;
    }
    getNextToken(); //eat )
    auto expr = ParseExpression();
    return std::make_shared<ForAST>(expr, init, cmp, step);
}

std::shared_ptr<InitialAST> Parser::ParseInitial() {
    getNextToken(); //eat Initial�ؼ���
    shared_ptr<ExprAST> expr = nullptr;
    if (curTokenKind != TokenKind::BeginKeyword) { //��˵�����е��б��ʽ
        expr = parsePrimary();
    }
    else { //��˵���ж��б��ʽ
        expr = ParseBegin();
    }
    return make_shared<InitialAST>(expr);
}

std::shared_ptr<ExprAST> Parser::ParseBegin() {
    LogP.addnote("->parsing a Begin...");
    getNextToken(); //eat Begin
    std::vector<shared_ptr<ExprAST>> Exprs;
    while (curTokenKind != TokenKind::EndKeyword) {
        if (m_offset == m_tokenVector.size() - 1 && curTokenKind != TokenKind::EndKeyword) {
            LE.addnote("expected 'end'", curToken.TL.m_tokenLine);
            break;
        }
        auto V = ParseExpression();
        if (V != nullptr)
            Exprs.push_back(V);
        getNextToken();
    }
    return std::move(std::make_shared<BeginAST>(Exprs));
}

std::shared_ptr<ExprAST> Parser::ParseIf() {
    LogP.addnote("->parsing a if...");
    getNextToken(); //eat If
    if (curTokenKind != TokenKind::OpenParenthesis) {
        LE.addnote("expected expression", curToken.TL.m_tokenLine);
        return nullptr;
    }
    auto cond = ParseParenExpr();
    auto expr = ParseExpression();
    return std::move(std::make_shared<IfAST>(cond, expr));
}

std::shared_ptr<ExprAST> Parser::ParseElse() {
    LogP.addnote("->parsing else...");
    getNextToken(); //eat else
    auto expr = ParseExpression();
    return std::move(std::make_shared<ElseAST>(expr));
}

std::shared_ptr<ExprAST> Parser::ParseParenExpr() { //����������for()
    getNextToken(); // eat (.
    shared_ptr<ExprAST> V = nullptr;
    switch (curTokenKind) {
    case TokenKind::Identifier: {
        if (!VariableInfo_umap.count(curToken.getTokenStr())) { //����ñ�ʶ�������ڣ���˵������δ�����ʶ��
            string tmpStr = "use of undeclared identifier '";
            tmpStr += curToken.getTokenStr();
            tmpStr += "'";
            LE.addnote(tmpStr, curToken.TL.m_tokenLine);
            return nullptr;
        }
        auto LHS = ParseIdentifierExpr(TokenKind::NullKeyword);
        if (curTokenKind != TokenKind::CloseParenthesis) { //�����Ϊ)��˵�����������ж�
            V = ParseCmpOpRHS(LHS);
        }
        break;
    }
    case TokenKind::IntegerLiteral:
        V = ParseNumber();
        break;
    default:
        V = parsePrimary();
        break;
    }
    if (curToken.getTokenKind() != TokenKind::CloseParenthesis) {
        LE.addnote("expected ')'", curToken.TL.m_tokenLine);
        return nullptr;
    }
    getNextToken(); // eat ).
    return V;
}

std::shared_ptr<ExprAST> Parser::ParseCmpOpRHS(std::shared_ptr<ExprAST> LHS) {
    string op;
    switch (curTokenKind) {
    case TokenKind::DoubleEquals:
    case TokenKind::LessThanEquals:
    case TokenKind::GreaterThanEquals:
    case TokenKind::GreaterThan:
    case TokenKind::LessThan:
        op = curToken.getTokenStr();
        break;
    default:
        LE.addnote("expected a compare operator", curToken.TL.m_tokenLine);
        return nullptr;
    }
    getNextToken(); //eat op
    shared_ptr<ExprAST> RHS = nullptr;
    switch (curTokenKind) {
    case TokenKind::Identifier: {
        if (!VariableInfo_umap.count(curToken.getTokenStr())) { //����ñ�ʶ�������ڣ���˵������δ�����ʶ��
            string tmpStr = "use of undeclared identifier '";
            tmpStr += curToken.getTokenStr();
            tmpStr += "'";
            LE.addnote(tmpStr, curToken.TL.m_tokenLine);
            return nullptr;
        }
        RHS = ParseIdentifierExpr(TokenKind::NullKeyword);
        break;
    }
    case TokenKind::IntegerLiteral:
        RHS = ParseNumber();
        break;
    default:
        LE.addnote("expected expression", curToken.TL.m_tokenLine);
        break;
    }
    LogP.addnote("->parsing a comparison expression");
    return make_shared<CmpExprAST>(op, LHS, RHS);
}

std::shared_ptr<ExprAST> Parser::ParseIdentifierExpr(TokenKind varType) {
    std::string IdName = curToken.getTokenStr();
    switch (varType) {
    case TokenKind::IntKeyword:
    case TokenKind::ShortIntKeyword:
    case TokenKind::LongIntKeyword:
    case TokenKind::LogicKeyword:
    case TokenKind::BitKeyword:
    case TokenKind::ByteKeyword:
    case TokenKind::IntegerKeyword:
    case TokenKind::RegKeyword:
    case TokenKind::PosEdgeKeyword:{
        if (VariableInfo_umap.count(IdName)) { //����ñ�ʶ���Ѿ����ڣ���˵���ظ�����
            LE.addnote("previous definition here", curToken.TL.m_tokenLine);
            return nullptr;
        }
        //���Ϊ�״ζ��壬��ˢ��VF�ṹ���ڵı�����Ϣ�������������
        VF.name = IdName;
        VF.kind = TokenKindtoString(varType);
        VariableInfo_umap[IdName] = VF;
        variableTypeFlag = TokenKind::NullKeyword; //����ʶ��flag��ԭ
        break;
    }
    case TokenKind::NullKeyword: //˵���Ƕ���������ñ�ʶ��������
        if (!VariableInfo_umap.count(IdName)) { //����ñ�ʶ�������ڣ���˵������δ�����ʶ��
            string tmpStr = "use of undeclared identifier '";
            tmpStr += IdName;
            tmpStr += "'";
            LE.addnote(tmpStr, curToken.TL.m_tokenLine);
            return nullptr;
        }
        break;
    default: //�����϶������͵Ĺؼ���
        LE.addnote("invaild type", curToken.TL.m_tokenLine);
        return nullptr;
        break;
    }
    getNextToken();
    auto V = std::make_shared<VariableExprAST>(IdName, VariableInfo_umap[IdName].kind); //��Ҫ�жϺ����Ƿ�Ϊ;��?
    return std::move(V);
}

std::shared_ptr<ExprAST> Parser::ParseExpression() {
    auto LHS = parsePrimary();
    if (!LHS)
        return nullptr;
    else if (curTokenKind == TokenKind::EndKeyword || curTokenKind == TokenKind::Semicolon || curTokenKind == TokenKind::CloseParenthesis)
        return LHS;
    return ParseBinOpRHS(0, std::move(LHS));
}

std::shared_ptr<ExprAST> Parser::ParseBinOpRHS(int ExprPrec, std::shared_ptr<ExprAST> LHS) { //������Ԫ���ʽ���Ұ벿��
    if (curTokenKind == TokenKind::Semicolon)
        return nullptr;
    while (1) {
        int curTokenPrec = GetTokPrecedence(); //��ȡ��ǰToken����������ȼ�
        if (curTokenPrec < ExprPrec)
            return LHS;
        string BinOp = curToken.getTokenStr();
        auto RHS = parsePrimary();
        if (!RHS)
            return nullptr;
        LogP.addnote("->parsing a Binary Expression...");
        int nextOpPrec = GetTokPrecedence(); //��ȡ��һ������������ȼ�
        if (curTokenPrec < nextOpPrec) {
            RHS = ParseBinOpRHS(curTokenPrec + 1, std::move(RHS));
            if (RHS == nullptr)
                return nullptr;
        }
        if (nextOpPrec == -1) {
            //m_offset--;
            return std::make_shared<BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS));
        }
        // Merge LHS/RHS.
        LHS = std::make_shared<BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS));
    }
}

void Parser::buildBinopPrecedence() {
    BinopPrecedence_umap["++"] = 5;
    BinopPrecedence_umap["--"] = 5;
    BinopPrecedence_umap["<"] = 10;
    BinopPrecedence_umap[">"] = 10;
    BinopPrecedence_umap["+"] = 20;
    BinopPrecedence_umap["-"] = 20;
    BinopPrecedence_umap["*"] = 40;
    BinopPrecedence_umap["/"] = 40;
    BinopPrecedence_umap["%"] = 40;
    BinopPrecedence_umap["&"] = 40;
    BinopPrecedence_umap["|"] = 40;
}

void Parser::buildTypeUset() {
    Type_uset.insert(TokenKind::IntKeyword);
    Type_uset.insert(TokenKind::StringKeyword);
    Type_uset.insert(TokenKind::BoolKeyword);
    Type_uset.insert(TokenKind::ShortIntKeyword);
    Type_uset.insert(TokenKind::LongIntKeyword);
    Type_uset.insert(TokenKind::CharKeyword);
    Type_uset.insert(TokenKind::VoidKeyword);
    Type_uset.insert(TokenKind::FloatKeyword);
    Type_uset.insert(TokenKind::DoubleKeyword);
}

void Parser::buildObjInstantiationUmap() {
    for (auto className : m_classNames) { //init
        ObjInstantiation_umap[className] = {};
    }
}

int Parser::GetTokPrecedence() {
    string curStr = curToken.getTokenStr();
    if (curStr == "=") {
        return 1;
    }
    else if (curStr == ";" || curStr == ")") {
        return -1;
    }
    else if (curStr != "+" //���Ӷ�curTokenKind���жϣ���ǿ������׳��
        && curStr != "-"
        && curStr != "*"
        && curStr != "/"
        && curStr != "%"
        && curStr != "&"
        && curStr != "|"
        && curStr != "<"
        && curStr != ">"
        && curStr != "++"
        && curStr != "--"
        && (curTokenKind == TokenKind::IntegerLiteral || curTokenKind == TokenKind::Identifier)
        ) {
        LE.addnote("expcted an operator", curToken.TL.m_tokenLine);
        return -1;
    }
    else if (BinopPrecedence_umap.count(curStr)) {
        //char chOp = curStr.at(0);
        int TokPrec = BinopPrecedence_umap[curStr];
        if (TokPrec <= 0)
            return -1;
        return TokPrec;
    }
    else {
        LE.addnote("expcted ';'", curToken.TL.m_tokenLine);
        return -1;
    }
}

std::shared_ptr<DefinitionAST> Parser::parseModule() {
    getNextToken();
    return ParseModuleDefinition(); //�˴��޷���ȷ�����ǽ���module�������Ƕ��壿
}

void Parser::handlModule() {
    if (parseModule()) {
        LogP.addnote("parsed a Module!");
    }
    else {
        getNextToken();
    }
}

void Parser::handlAlways_ff() {
    getNextToken();
    if (ParseAlways_ff()) {
        LogP.addnote("parsed Always_ff!");
    }
    else {
        getNextToken();
    }
}

void Parser::handlAlways_comb() {
    getNextToken();
    if (ParseAlways_comb()) {
        LogP.addnote("parsed Always_comb!");
    }
    else {
        getNextToken();
    }
}

void Parser::handlFunc() {
    cout << "Parsing function:" << curToken.getTokenStr() << "()..." << endl;
    Token nextToken = m_tokenVector[m_offset];
    TokenKind nextTokenKind = nextToken.getTokenKind();
    if (nextTokenKind != TokenKind::OpenParenthesis) { //�����ʱ��һ��Token��'('����˵����ʱ��һ������
        return;
    }
    while (curTokenKind != TokenKind::CloseParenthesis) { //����func()�����в�������������umlʱ��ͼ���岻��(��ʱ�����ǲ�����ƺ����������)
        getNextToken();  //eat
    }
    getNextToken(); //eat )
    getNextToken(); //eat {
    while (curTokenKind != TokenKind::CloseBrace) {
        mainParser();
    }
}

void Parser::handlObj() {
    Token nextToken = m_tokenVector[m_offset];
    TokenKind nextTokenKind = nextToken.getTokenKind();
    Token n_nextToken = m_tokenVector[m_offset + 1]; //next��next
    TokenKind n_nextTokenKind = n_nextToken.getTokenKind();
    //case1: A a();
    if (nextTokenKind == TokenKind::Identifier) {
        ObjInstantiation_umap[curToken.getTokenStr()].emplace_back(nextToken.getTokenStr()); //add ObjInstantiation_umap
        while (curTokenKind != TokenKind::Semicolon) {
            getNextToken();
        }
        getNextToken(); //eat ;
    }
    //case2: A *a = new A();
    else if (nextTokenKind == TokenKind::Star) {
        ObjInstantiation_umap[curToken.getTokenStr()].emplace_back(n_nextToken.getTokenStr()); //add ObjInstantiation_umap
        while (curTokenKind != TokenKind::Semicolon) {
            getNextToken();
        }
        getNextToken(); //eat ;
    }
    //case3: a.work();
    else if (nextTokenKind == TokenKind::Dot) {
        //��ǰcurToken��a,��Ҫͨ��a�ҵ�
        FuncCallInformation FC;
        FC.invokeClassName = findClassName(curToken.getTokenStr());
        getNextToken(); //eat Indentifier,like 'a'
        getNextToken(); //eat Dot;
        FC.FuncName = curToken.getTokenStr();
        //FuncCallInformation_umap[getClassCounter()] = FC;
        m_pCList->addFuncCallInfo(FC);
        getNextToken(); //eat Indentifier, like 'working'
        getNextToken(); //eat (
        getNextToken(); //eat )
        getNextToken(); //eat ;
        cout << "parseing obj call function..." << endl;
        cout << "---->" << FC.invokeClassName << "." << FC.FuncName << "()" << endl;
    }
    //case4: a->work();
    else if (nextTokenKind == TokenKind::MemberPointerAccess) {
        //��ǰcurToken��a,��Ҫͨ��a�ҵ�
        FuncCallInformation FC;
        FC.invokeClassName = findClassName(curToken.getTokenStr());
        getNextToken(); //eat Indentifier,like 'a'
        getNextToken(); //eat '->'
        FC.FuncName = curToken.getTokenStr();
        //FuncCallInformation_umap[getClassCounter()] = FC;
        m_pCList->addFuncCallInfo(FC);
        getNextToken(); //eat Indentifier, like 'working'
        getNextToken(); //eat (
        getNextToken(); //eat )
        getNextToken(); //eat ;
        cout << "parseing obj call function..." << endl;
        cout << "---->" << FC.invokeClassName << "->" << FC.FuncName << "()" << endl;
    }
    else {
        ParseExpression();
    }
}

void Parser::handInitial() {
    getNextToken();
    if (ParseInitial()) {
        LogP.addnote("parsed Initial!");
    }
    else {
        getNextToken();
    }
}

void Parser::handInclude() {
    getNextToken(); //eat include;
    getNextToken(); //eat "";
    getNextToken();
}

void Parser::showErrorInformation() {
    cout << "------------<ErrorInformation>-----------" << endl;
    cout << LE.errorNotes.size() << " errors..." << endl;
    for (auto errorNote : LE.errorNotes) {
        cout << errorNote;
    }
}

void Parser::showParserInformation() {
    cout << "-----------<ParserInformation>-----------" << endl;
    for (auto note : LogP.parserNotes) {
        cout << note << endl;
    }
}

void Parser::showVariableInformation() {
    cout << "-----------<VariableInformation>---------" << endl;
    cout << "Name--------Tpye" << endl;
    for (auto varInfo : VariableInfo_umap) {
        cout <<"<" << varInfo.second.name << ">-----<" << varInfo.second.kind << ">" << varInfo.second.content << endl;
    }
}

string Parser::findClassName(string targetStr) {
    for (auto obj : ObjInstantiation_umap) {
        for (int i = 0; i < obj.second.size(); i++) {
            if (targetStr == obj.second[i]) {
                return obj.first;
            }
        }
    }
    return "error";
}