#include "Parser.h"
Parser::Parser(unordered_map<string, vector<Token>> hTokenFlows, unordered_map<string, vector<Token>> cppTokenFlows, vector<Token> tokenVector, vector<string> classNames, ClassList* pCList, string curFileName,string startClassName)
    :m_hTokenFlows(hTokenFlows),
    m_cppTokenFlows(cppTokenFlows),
    m_tokenVector(tokenVector),
    variableTypeFlag(TokenKind::NullKeyword),
    m_classNames(classNames),
    m_pCList(pCList),
    m_curFileName(curFileName),
    m_startClassName(startClassName)
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
    //showErrorInformation();
    //showVariableInformation();
}
//todo 尖括号
void Parser::mainParser() {
    int tmpSize = m_tokenVector.size() - 1;
    while(m_offset < tmpSize && m_tokenVector.size() != 0) { //warning:�˴�m_offset���޷������ͣ���int���ͱȽϻ�ʹtmpSizeҲͳһΪ�޷������ͣ�������tmpSize<0ʱ��ת���������������Ҫ��һ���ж�
        cout<<curToken.getTokenStr()+"hahhhahahhahha";
        string a = curToken.getTokenStr();
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
        case TokenKind::IncludeKeyword:
            handInclude();
            break;
        case TokenKind::WhileKeyword:
            ParseWhile();
            break;
        case TokenKind::ForKeyword:
            ParseFor();
            break;
        case TokenKind::IfKeyword:
            ParseIf();
            break;
        case TokenKind::ElseKeyword:
            ParseElse();
            break;
        default:
            if (Type_uset.count(curTokenKind)) { //�����Type���У���˵����ǰtokenΪint�����͹ؼ���
                cout<<curToken.getTokenStr()+"debug";
                getNextToken(); //eat type
                handlFunc();
                
            }
            if (curTokenKind == TokenKind::EndOfFile) {
                    m_offset = m_tokenVector.size();
                    break;
            }
            if (m_offset >= tmpSize){
                int i = m_offset;
                ParseExpression();
                break;
            }
            if (curTokenKind == TokenKind::IfKeyword || curTokenKind == TokenKind::WhileKeyword || curTokenKind == TokenKind::ElseKeyword || curTokenKind == TokenKind::ForKeyword) {
                parsePrimary();
            }
            if (curTokenKind == TokenKind::CloseBrace || curTokenKind == TokenKind::CloseBracket || curTokenKind == TokenKind::CloseParenthesis || curTokenKind == TokenKind::Semicolon) {
                getNextToken();
            }
            if(m_offset < tmpSize)
            ParseExpression();
            break;
        }
    }
}

void Parser::getNextToken() {
    if (m_offset >= m_tokenVector.size())
        return;
    curToken = m_tokenVector[++m_offset];
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
    case TokenKind::DoubleKeyword:
        LogP.addnote("->parsing a DoubleVariable...");
        variableTypeFlag = TokenKind::DoubleKeyword;
        break;
    case TokenKind::CharKeyword:
        LogP.addnote("->parsing a CharVariable...");
        variableTypeFlag = TokenKind::CharKeyword;
        break;
    case TokenKind::StringKeyword:
        LogP.addnote("->parsing a StringVariable...");
        variableTypeFlag = TokenKind::StringKeyword;
        break;
    case TokenKind::VoidKeyword:
        LogP.addnote("->parsing a VoidVariable...");
        variableTypeFlag = TokenKind::VoidKeyword;
        break;
    case TokenKind::BoolKeyword:
        LogP.addnote("->parsing a BoolVariable...");
        variableTypeFlag = TokenKind::BoolKeyword;
        break;
    case TokenKind::FloatKeyword:
        LogP.addnote("->parsing a FloatVariable...");
        variableTypeFlag = TokenKind::FloatKeyword;
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
    case TokenKind::WhileKeyword: {
        LogP.addnote("parsing while:");
        auto V = ParseWhile();
        return std::move(V);
    }
    case TokenKind::ReturnKeyword: {
        LogP.addnote("parsing return:");
        handlReturn();
        return nullptr;
    }
    case TokenKind::Identifier:
        return ParseIdentifierExpr(TokenKind::NullKeyword);
    case TokenKind::DoublePlus:
    case TokenKind::DoubleMinus: {
        auto V = std::make_shared<NumberExprAST>(1);
        getNextToken();//eat op
        return std::move(V);
    }
    case TokenKind::CloseBrace:
        return nullptr;
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
/*todo list:1：增加switch解析d 增加while dowhile （foreach）等的支持；2：循环和判断数据加载；3：自调用数据加载和创建；4：if和else和loop的两种情况(花括号省略) 5:对elseif等语句的支持:5：隐藏的a.work
 * 需要判断当前类的成员函数，并解析（自调用情况或者头文件里的静态成员函数）实现对重名实例化的支持；不能实现直接的类解析
方案设想：1:自调用数据通过后继节点的层数判断 ，初始值设置为0，每次将自身直接后继节点的调用层数+1；一种减少循环的方法，classlist里面维护数组，一旦判断direct节点以后即将此节点加入禁用清单
 2：标记自身调用，invoke ==call 标记一次自调用并继续上找，否则不找了
 3 标记激活周期 根据子孙节点的序列号更新激活周期（最好在activate里激活）
active维护激活的序列号和此时的调用层数「
 方案一，面向箭头生成 维护单个call的自调用层数 方法如图方法2
 方案二、 面向类 维护一个表 key：激活时间 value：层数 实现方案：同样针对类更新激活时间，并imvoke ==call 寻找层数
 」
 4. 标记if/else 遇到if后判断是否需要括号匹配，若没有，解析语句，更新清单，若有执行括号匹配算法，分别统计执行前后的size，确定包含的
 5. 标记loop 同理
 Loop表维护
 ：size 从而维护了
 */

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
    LoopInformation LP;
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
    LP.LoopCondition =  m_tokenVector[m_offset-1].getTokenStr();
    getNextToken(); //eat ;
    auto step = ParseExpression();
    if (curTokenKind != TokenKind::CloseParenthesis) {
        LE.addnote("expected ')'", curToken.TL.m_tokenLine);
        return nullptr;
    }
    getNextToken(); //eat )
    auto expr = ParseExpression();
    if (curTokenKind != TokenKind::OpenBrace) { //while��ʽ��ȱ��expected '{'
        return nullptr;
    }
    getNextToken(); //eat '{'
    vector<shared_ptr<ExprAST>> exprs;

    int start =m_pCList->getFuncCallInfo().size();
    while (curTokenKind != TokenKind::CloseBrace) {
        auto expr = ParseExpression();
        exprs.push_back(expr);
        if (curTokenKind == TokenKind::Semicolon) {
            getNextToken(); //eat ';'
        }
    }
    int end =m_pCList->getFuncCallInfo().size();
    for(int i = start; i < end; i++){
        LP.timeLine.push_back(i);
        LP.loopIcludeClassName.push_back(m_pCList->getFuncCallInfo().at(i).callClassName);
        LP.loopIcludeClassName.push_back(m_pCList->getFuncCallInfo().at(i).invokeClassName);
    }
    m_pCList->addLoopInfo(LP);
    getNextToken(); //eat '}'
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
std::shared_ptr<ExprAST> Parser::HandleAlt(){

}//增加新特性switch的支持
std::shared_ptr<ExprAST> Parser::ParseIf() {
    LogP.addnote("->parsing a if...");
    getNextToken(); //eat If
    if (curTokenKind != TokenKind::OpenParenthesis) {
        LE.addnote("expected expression", curToken.TL.m_tokenLine);
        return nullptr;
    }
    vector<shared_ptr<ExprAST>> exprs;
    auto cond = ParseParenExpr();
    AltInformation AT;
    AT.altCondition = m_tokenVector[m_offset-2].getTokenStr();
    int start =m_pCList->getFuncCallInfo().size();
    if (curTokenKind != TokenKind::OpenBrace) {
        auto expr = ParseExpression();
        exprs.push_back(expr);
    }

    while (curTokenKind != TokenKind::CloseBrace) {
        auto expr = ParseExpression();
        exprs.push_back(expr);
        if (curTokenKind == TokenKind::Semicolon) {
            getNextToken(); //eat ';'
        }
    }
    int end =m_pCList->getFuncCallInfo().size();
    for(int i = start; i < end; i++){
        AT.timeLine.push_back(i+1);
        AT.altIncludeClassName.push_back(m_pCList->getFuncCallInfo().at(i+1).callClassName);
        AT.altIncludeClassName.push_back(m_pCList->getFuncCallInfo().at(i+1).invokeClassName);
    }
    m_pCList->addAltInfo(AT);
    getNextToken(); //eat '}'
    return std::move(std::make_shared<IfAST>(cond, exprs));
}

std::shared_ptr<ExprAST> Parser::ParseElse() {
    LogP.addnote("->parsing else...");
    getNextToken(); //eat else
    shared_ptr<ExprAST> cond = nullptr;
    AltInformation AT;
    string elseCondition ="NULLCONDITION";
    if(curTokenKind == TokenKind:: IfKeyword){
        if (curTokenKind != TokenKind::OpenParenthesis) {
            LE.addnote("expected expression", curToken.TL.m_tokenLine);
            return nullptr;
        }
         cond = ParseParenExpr();
        elseCondition = m_tokenVector[m_offset-2].getTokenStr();
    }
    vector<shared_ptr<ExprAST>> exprs;
    int start =m_pCList->getFuncCallInfo().size();
    if (curTokenKind != TokenKind::OpenBrace) {
        auto expr = ParseExpression();
        exprs.push_back(expr);
    }
    while (curTokenKind != TokenKind::CloseBrace) {
        auto expr = ParseExpression();
        exprs.push_back(expr);
    }
    int end =m_pCList->getFuncCallInfo().size();
    for(int i = start; i < end; i++){
        string a;
        a = m_pCList->getFuncCallInfo().at(i).invokeClassName;
        AT.timeLine.push_back(i+1);
        AT.altIncludeClassName.push_back(m_pCList->getFuncCallInfo().at(i+1).callClassName);
        AT.altIncludeClassName.push_back(m_pCList->getFuncCallInfo().at(i+1).invokeClassName);
    }
    int elseStartPosition =start+1;
    m_pCList->modifyAltInfo(AT,elseStartPosition,elseCondition);
    return std::move(std::make_shared<ElseAST>(cond,exprs));
}

std::shared_ptr<ExprAST> Parser::ParseParenExpr() { //����������for()
    getNextToken();// eat (.
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
    case TokenKind::PosEdgeKeyword:
    case TokenKind::StringKeyword:
    case TokenKind::BoolKeyword:
    case TokenKind::CharKeyword:
    case TokenKind::VoidKeyword:
    case TokenKind::FloatKeyword:
    case TokenKind::DoubleKeyword:{
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
    case TokenKind::NullKeyword: {//˵���Ƕ���������ñ�ʶ��������
        TokenKind nextTokenKind = m_tokenVector[m_offset+1].getTokenKind();
        //TokenKind n_nextTokenKind = m_tokenVector[m_offset + 1].getTokenKind();
        if (isClassName(curToken.getTokenStr()) || nextTokenKind == TokenKind::OpenParenthesis || nextTokenKind == TokenKind::Star || nextTokenKind == TokenKind::Dot || nextTokenKind == TokenKind::MemberPointerAccess) { //��ǰΪһ������
            auto V = handlObj(); //��ʱ����return
            //getNextToken(); //eat
            return V; //���ܷ���һ����
        }
        else if (!VariableInfo_umap.count(IdName) && nextTokenKind != TokenKind::OpenParenthesis) { //����ñ�ʶ�������ڣ���˵������δ�����ʶ��
            string tmpStr = "use of undeclared identifier '";
            tmpStr += IdName;
            tmpStr += "'";
            LE.addnote(tmpStr, curToken.TL.m_tokenLine);
            return nullptr;
        }

        break;
    }
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
    string a = curToken.getTokenStr();
    if (!LHS) {
        if(curTokenKind != TokenKind::CloseBrace)
            getNextToken();
        return nullptr;
    }
    else if (curTokenKind == TokenKind::EndKeyword || curTokenKind == TokenKind::Semicolon || curTokenKind == TokenKind::CloseParenthesis || curTokenKind == TokenKind::CloseBrace) {
        getNextToken();
        return LHS;
    }
    else if (curTokenKind == TokenKind::ReturnKeyword) {
        handlReturn();
        return LHS;
    }
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
    cout << "Parsing function: " << curToken.getTokenStr()<<" ";
    Token curToken1 = m_tokenVector[m_offset];
    TokenKind curTokenKind1 = curToken1.getTokenKind();
    if (curTokenKind1 == TokenKind::Identifier && isClassName(curToken1.getTokenStr())) {
        cout << curToken1.getTokenStr() << "::";
        getNextToken(); //eat ClassName
        getNextToken(); //eat ':'
        getNextToken();
    }
    cout << curToken1.getTokenStr() << "()..." << endl;
    getNextToken(); //eat Identifier
    if (curTokenKind != TokenKind::OpenParenthesis) { //�����ʱ��һ��Token��'('����˵����ʱ��һ������
        --m_offset;
        curToken = m_tokenVector[--m_offset];
        curTokenKind = curToken.getTokenKind();
        cout << "-->Not a Funciton..." << endl;;
        return;
    }
    while (curTokenKind != TokenKind::CloseParenthesis) { //����func()�����в�������������umlʱ��ͼ���岻��(��ʱ�����ǲ�����ƺ����������)
        getNextToken();  //eat
    }
    getNextToken(); //eat )
    getNextToken(); //eat {
    while (curTokenKind != TokenKind::CloseBrace) {
        ParseExpression();
        if (curTokenKind == TokenKind::Semicolon) {
            getNextToken(); //eat ;
        }
    }
}

std::shared_ptr<FuncAST> Parser::handlObj() {
    if (isFuncName(curToken.getTokenStr())) { //work()
        FuncCallInformation FC;
        //todo 中间能否空行，全局变量区分,结构体区分
        FC.invokeClassName = m_curFileName.substr(0, m_curFileName.size()-4);
        FC.callClassName =m_startClassName;
        FC.FuncName = curToken.getTokenStr();
        string funcname =FC.FuncName;
        int parentoffset =1;
        if (m_tokenVector[m_offset + parentoffset].getTokenKind()==TokenKind::OpenParenthesis){
            while (m_tokenVector[m_offset + parentoffset].getTokenKind()!=TokenKind::CloseParenthesis){
                 funcname =funcname+ m_tokenVector[m_offset + parentoffset].getTokenStr();
                ++parentoffset;
            }
            funcname= funcname + m_tokenVector[m_offset + parentoffset].getTokenStr();
            FC.FuncName =funcname;
        }
        cout << "parseing internal function..." << endl;
        cout << "---->" << FC.FuncName << "()" << endl;
        m_pCList->addFuncCallInfo(FC);
        while (curTokenKind != TokenKind::Semicolon) {
            getNextToken();
        }
       // getNextToken(); //eat ;
        vector<Token> targetTokenFlows = filterTokenFlow(FC.FuncName, FC.invokeClassName + ".cpp");
        int curFuncCallOrder = (m_pCList->getFuncCallInfo()).size();
       // if (FC.FuncName != m_tokenVector[4].getTokenStr())
        Parser dfsPar(m_hTokenFlows, m_cppTokenFlows, targetTokenFlows, m_classNames, m_pCList, FC.invokeClassName + ".cpp",m_startClassName);
        int afterDfsCallOrder = (m_pCList->getFuncCallInfo()).size();
        //统计递归次数
        unordered_map<int,int> tmpCurDescendantsSequenceMap;
        for ( int i = curFuncCallOrder+1; i <= afterDfsCallOrder; i++){
            tmpCurDescendantsSequenceMap[i]=i;
        }
        //从子孙节点中提纯子节点
        m_pCList-> modifyDescendantsSequence (curFuncCallOrder,tmpCurDescendantsSequenceMap);
        m_pCList-> modifyDirectDescendantsSequence (curFuncCallOrder,tmpCurDescendantsSequenceMap);
        for (int i = curFuncCallOrder+1; i <= afterDfsCallOrder; i++){
            auto tmpDescendantsSequenceMap =(m_pCList->getFuncCallInfo()).at(i).descendantsSequence;
            for(auto j:tmpDescendantsSequenceMap){
                m_pCList-> DeleteDirectDDescendantsSequenceEnum(curFuncCallOrder,j.first);
            }
        }
        auto tmpDescendantsSequenceMapFetch = (m_pCList->getFuncCallInfo()).at(curFuncCallOrder).directDescendantsSequence;
        //将子节点的callClassName设置为当前类名
        for (auto i =tmpDescendantsSequenceMapFetch.begin(); i!= tmpDescendantsSequenceMapFetch.end();++i ){
            m_pCList->setCallClassName(i->first,curFuncCallOrder);
        }
        //装载激活相关信息
        auto activationClassName=m_pCList->getFuncCallInfo().at(curFuncCallOrder).invokeClassName;
        m_pCList->modifyClassActivationInfo(activationClassName,curFuncCallOrder,1);
        for (int i =curFuncCallOrder+1;i<=afterDfsCallOrder;i++){
            if(m_pCList->getActivationInfo().at(activationClassName).find(i)== m_pCList->getActivationInfo().at(activationClassName).end()){
                m_pCList->modifyClassActivationInfo(activationClassName,i,1);
            }
            else{
                auto tmpDescendantsSequenceMap =(m_pCList->getFuncCallInfo()).at(i).descendantsSequence;
                for(auto j:tmpDescendantsSequenceMap){
                    int k=m_pCList->getActivationInfo().at(activationClassName).at(j.first)+1;
                    m_pCList->modifyClassActivationInfo(activationClassName,i,k);
                }
            }
        }
        // vector<Token> targetTokenFlows2 = filterTokenFlow(FC.FuncName, FC.invokeClassName+".cpp");
        // Parser (m_hTokenFlows, m_cppTokenFlows, targetTokenFlows2, m_classNames, m_pCList, FC.invokeClassName + ".cpp");
        return make_shared<FuncAST>(FC.FuncName);
    }
    cout<<curToken.getTokenStr();
    Token nextToken = m_tokenVector[m_offset+1];
    TokenKind nextTokenKind = nextToken.getTokenKind();
    Token n_nextToken = m_tokenVector[m_offset + 2]; //next��next
    TokenKind n_nextTokenKind = n_nextToken.getTokenKind();
    //case1: A a();
    if (nextTokenKind == TokenKind::Identifier) {
        ObjInstantiation_umap[curToken.getTokenStr()].emplace_back(nextToken.getTokenStr()); //add ObjInstantiation_umap
        while (curTokenKind != TokenKind::Semicolon) {
            cout<<curToken.getTokenStr()+"---------";
            getNextToken();
        }
        cout<<"next";
        return make_shared<FuncAST>(nextToken.getTokenStr());
    }
    //case2: A *a = new A();
    else if (nextTokenKind == TokenKind::Star) {
        ObjInstantiation_umap[curToken.getTokenStr()].emplace_back(n_nextToken.getTokenStr()); //add ObjInstantiation_umap
        while (curTokenKind != TokenKind::Semicolon) {
            getNextToken();
        }
        return make_shared<FuncAST>(n_nextToken.getTokenStr());
    }
    //case3: a.work();
    else if (nextTokenKind == TokenKind::Dot) {
        //��ǰcurToken��a,��Ҫͨ��a�ҵ�
        FuncCallInformation FC;
        FC.invokeClassName = findClassName(curToken.getTokenStr());
        getNextToken(); //eat Indentifier,like 'a'
        getNextToken(); //eat Dot;
        FC.FuncName = curToken.getTokenStr();
        string funcname =FC.FuncName;
        int parentoffset =1;
        if (m_tokenVector[m_offset + parentoffset].getTokenKind()==TokenKind::OpenParenthesis){
            while (m_tokenVector[m_offset + parentoffset].getTokenKind()!=TokenKind::CloseParenthesis){
                 funcname =funcname+ m_tokenVector[m_offset + parentoffset].getTokenStr();
                ++parentoffset;
            }
            funcname= funcname + m_tokenVector[m_offset + parentoffset].getTokenStr();
            FC.FuncName =funcname;
        }
        FC.callClassName =m_startClassName;
        m_pCList->addFuncCallInfo(FC);

        while (curTokenKind != TokenKind::Semicolon) {
            getNextToken();
        }
        //getNextToken(); //eat ;
        cout << "parseing obj call function..." << endl;
        cout << "---->" << FC.invokeClassName << "." << FC.FuncName << "()" << endl;
        int curFuncCallOrder = (m_pCList->getFuncCallInfo()).size();
        vector<Token> targetTokenFlows = filterTokenFlow(FC.FuncName, FC.invokeClassName+".cpp");
        Parser dfs(m_hTokenFlows, m_cppTokenFlows, targetTokenFlows, m_classNames, m_pCList, FC.invokeClassName + ".cpp",m_startClassName);
        int afterDfsCallOrder = (m_pCList->getFuncCallInfo()).size();
        //统计递归次数
        unordered_map<int,int> tmpCurDescendantsSequenceMap;
        for ( int i = curFuncCallOrder+1; i <= afterDfsCallOrder; i++){
            tmpCurDescendantsSequenceMap[i]=i;
        }
        //从子孙节点中提纯子节点
        m_pCList-> modifyDescendantsSequence (curFuncCallOrder,tmpCurDescendantsSequenceMap);
        m_pCList-> modifyDirectDescendantsSequence (curFuncCallOrder,tmpCurDescendantsSequenceMap);
        for (int i = curFuncCallOrder+1; i <= afterDfsCallOrder; i++){
             auto tmpDescendantsSequenceMap =(m_pCList->getFuncCallInfo()).at(i).descendantsSequence;
                for(auto j:tmpDescendantsSequenceMap){
                   m_pCList-> DeleteDirectDDescendantsSequenceEnum(curFuncCallOrder,j.first);
                }
            }
        auto tmpDescendantsSequenceMapFetch = (m_pCList->getFuncCallInfo()).at(curFuncCallOrder).directDescendantsSequence;
        //将子节点的callClassName设置为当前类名
        for (auto i =tmpDescendantsSequenceMapFetch.begin(); i!= tmpDescendantsSequenceMapFetch.end();++i ){
           m_pCList->setCallClassName(i->first,curFuncCallOrder);
        }
        //装载激活相关信息
       auto activationClassName=m_pCList->getFuncCallInfo().at(curFuncCallOrder).invokeClassName;
        m_pCList->modifyClassActivationInfo(activationClassName,curFuncCallOrder,1);
        for (int i =curFuncCallOrder+1;i<=afterDfsCallOrder;i++){
            if(m_pCList->getActivationInfo().at(activationClassName).find(i)== m_pCList->getActivationInfo().at(activationClassName).end()){
                m_pCList->modifyClassActivationInfo(activationClassName,i,1);
            }
            else{
                auto tmpDescendantsSequenceMap =(m_pCList->getFuncCallInfo()).at(i).descendantsSequence;
                for(auto j:tmpDescendantsSequenceMap){
                    int k=m_pCList->getActivationInfo().at(activationClassName).at(j.first)+1;
                    m_pCList->modifyClassActivationInfo(activationClassName,i,k);
                }
            }
        }
       // vector<Token> targetTokenFlows2 = filterTokenFlow(FC.FuncName, FC.invokeClassName+".cpp");
       // Parser (m_hTokenFlows, m_cppTokenFlows, targetTokenFlows2, m_classNames, m_pCList, FC.invokeClassName + ".cpp");
        return make_shared<FuncAST>(FC.FuncName);
    }
    //case4: a->work();
    else if (nextTokenKind == TokenKind::MemberPointerAccess) {
        //��ǰcurToken��a,��Ҫͨ��a�ҵ�
        FuncCallInformation FC;
        FC.invokeClassName = findClassName(curToken.getTokenStr());
        getNextToken(); //eat Indentifier,like 'a'
        getNextToken(); //eat '->'
        FC.FuncName = curToken.getTokenStr();
        string funcname =FC.FuncName;
        Token nextTokens = m_tokenVector[m_offset + 1];
        int parentoffset =1;
        if (m_tokenVector[m_offset + parentoffset].getTokenKind()==TokenKind::OpenParenthesis){
            // funcname = funcname +" ";
            while (m_tokenVector[m_offset + parentoffset].getTokenKind()!=TokenKind::CloseParenthesis){
                 funcname =funcname+ m_tokenVector[m_offset + parentoffset].getTokenStr();
                ++parentoffset;
            }
            funcname= funcname + m_tokenVector[m_offset + parentoffset].getTokenStr();
            FC.FuncName =funcname;
        }
        FC.callClassName =m_startClassName;
        int curFuncCallOrder21 = (m_pCList->getFuncCallInfo()).size();
        m_pCList->addFuncCallInfo(FC);
        int curFuncCallOrder22 = (m_pCList->getFuncCallInfo()).size();
        while (curTokenKind != TokenKind::Semicolon) {
            getNextToken();
        }
        //getNextToken(); //eat ;
        cout << "parseing obj call function..." << endl;
        cout << "---->" << FC.invokeClassName << "->" << FC.FuncName << "()" << endl;
        //
        int curFuncCallOrder = (m_pCList->getFuncCallInfo()).size();
        vector<Token> targetTokenFlows = filterTokenFlow(FC.FuncName, FC.invokeClassName + ".cpp");
        Parser dfs(m_hTokenFlows, m_cppTokenFlows, targetTokenFlows, m_classNames, m_pCList, FC.invokeClassName + ".cpp",m_startClassName);
        int afterDfsCallOrder = (m_pCList->getFuncCallInfo()).size();
        //统计递归次数
        unordered_map<int,int> tmpCurDescendantsSequenceMap;
        for ( int i = curFuncCallOrder+1; i <= afterDfsCallOrder; i++){
            tmpCurDescendantsSequenceMap[i]=i;
        }
        //从子孙节点中提纯子节点
        m_pCList-> modifyDescendantsSequence (curFuncCallOrder,tmpCurDescendantsSequenceMap);
        m_pCList-> modifyDirectDescendantsSequence (curFuncCallOrder,tmpCurDescendantsSequenceMap);
        for (int i = curFuncCallOrder+1; i <= afterDfsCallOrder; i++){
            auto tmpDescendantsSequenceMap =(m_pCList->getFuncCallInfo()).at(i).descendantsSequence;
            for(auto j:tmpDescendantsSequenceMap){
                m_pCList-> DeleteDirectDDescendantsSequenceEnum(curFuncCallOrder,j.first);
            }
        }
        auto tmpDescendantsSequenceMapFetch = (m_pCList->getFuncCallInfo()).at(curFuncCallOrder).directDescendantsSequence;
        //将子节点的callClassName设置为当前类名
        for (auto i =tmpDescendantsSequenceMapFetch.begin(); i!= tmpDescendantsSequenceMapFetch.end();++i ){
            m_pCList->setCallClassName(i->first,curFuncCallOrder);
        }
        if (!((m_pCList->getFuncCallInfo()).at(curFuncCallOrder).callClassName.size()) ){
            (m_pCList->getFuncCallInfo()).at(curFuncCallOrder).callClassName ="Main";
        }
        //装载激活相关信息
        auto activationClassName=m_pCList->getFuncCallInfo().at(curFuncCallOrder).invokeClassName;
        m_pCList->modifyClassActivationInfo(activationClassName,curFuncCallOrder,1);
        for (int i =curFuncCallOrder+1;i<=afterDfsCallOrder;i++){
            if(m_pCList->getActivationInfo().at(activationClassName).find(i)== m_pCList->getActivationInfo().at(activationClassName).end()){
                m_pCList->modifyClassActivationInfo(activationClassName,i,1);
            }
            else{
                auto tmpDescendantsSequenceMap =(m_pCList->getFuncCallInfo()).at(i).descendantsSequence;
                for(auto j:tmpDescendantsSequenceMap){
                    int k=m_pCList->getActivationInfo().at(activationClassName).at(j.first)+1;
                    m_pCList->modifyClassActivationInfo(activationClassName,i,k);
                }
            }
        }

        if (!((m_pCList->getFuncCallInfo()).at(curFuncCallOrder).callClassName.size()) ){
            (m_pCList->getFuncCallInfo()).at(curFuncCallOrder).callClassName ="Main";
        }
        return make_shared<FuncAST>(FC.FuncName);
    }
    else {
        ParseExpression();
    }
    return nullptr;
}

std::shared_ptr<WhileAST>  Parser::ParseWhile() {
    cout << "Parsing while..." << endl;
    getNextToken(); //eat while
    if (curTokenKind != TokenKind::OpenParenthesis) { //while��ʽ��ȱ��expected '('
        return nullptr;
    }
    getNextToken(); //eat '('
    auto LHS = ParseIdentifierExpr(TokenKind::NullKeyword);
    auto cmp = ParseCmpOpRHS(LHS);
    getNextToken(); //eat ')'
    vector<shared_ptr<ExprAST>> exprs;
    LoopInformation LP;
    LP.LoopCondition =  m_tokenVector[m_offset-1].getTokenStr();
    int start =m_pCList->getFuncCallInfo().size();
    getNextToken(); //eat '{'

    while (curTokenKind != TokenKind::CloseBrace) {
        auto expr = ParseExpression();
        exprs.push_back(expr);
        if (curTokenKind == TokenKind::Semicolon) {
            getNextToken(); //eat ';'
        }
    }
    int end =m_pCList->getFuncCallInfo().size();
    for(int i = start; i < end; i++){
        LP.timeLine.push_back(i);
        LP.loopIcludeClassName.push_back(m_pCList->getFuncCallInfo().at(i).callClassName);
        LP.loopIcludeClassName.push_back(m_pCList->getFuncCallInfo().at(i).invokeClassName);
    }
    m_pCList->addLoopInfo(LP);
    getNextToken(); //eat '}'
    return make_shared<WhileAST>(cmp, exprs);
}
std::shared_ptr<WhileAST>  Parser::ParseDoWhile(){

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
    if(curTokenKind == TokenKind::LessThan);{
        getNextToken();
        getNextToken();
    }
    getNextToken(); //eat "";
}
void Parser::handlReturn() {
    LogP.addnote("parsed return...");
    while (curTokenKind != TokenKind::Semicolon) {
        getNextToken();
    }
    getNextToken(); //eat ';'
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

/*
function: �ҵ�a.work()��a��class��.cpp�ļ��ʷ��������tokenflow��ɸѡ������work()��token
param: Ŀ�꺯������Ŀ��.cpp�ļ���
return: ɸѡ�õ���tokenflowfilte
//todo 静态成员函数 this指针 filter文件todo列表初始化
*/
vector<Token> Parser::filterTokenFlow(string targetFuncName, string targetfFleName) {
    vector<Token> MresTokenFlow;
    vector<Token> resTokenFlow;
    for (int i = 0; i < m_cppTokenFlows[targetfFleName].size(); i++) { //����ɨ���.cpp�ļ�������tokenflow
        Token nextFourToken;
        Token curToken = m_cppTokenFlows[targetfFleName][i];
        if (i <= m_cppTokenFlows[targetfFleName].size() - 4) {
            nextFourToken = m_cppTokenFlows[targetfFleName][i+4];
        }
        //vector<Token> resTokenFlow;
        if (Type_uset.count(curToken.getTokenKind()) && nextFourToken.getTokenStr() == targetFuncName) { //�ҵ�Ŀ�꺯��tokenλ��
            for (int j = i; j < m_cppTokenFlows[targetfFleName].size(); j++) {
                Token tmpToken = m_cppTokenFlows[targetfFleName][j];
                //vector<Token> resTokenFlow;
                //resTokenFlow.push_back(create(TokenKind::VoidKeyword, 1, 1, "void"));
                MresTokenFlow.push_back(tmpToken);
                 if (m_cppTokenFlows[targetfFleName][j].getTokenKind() == TokenKind::CloseBrace) {
                    i++;
                    break;
                }
            }
            break;
        }
    }
    resTokenFlow = MresTokenFlow; //���ڱ���ʱ���ű���ܳ������������bug���ʴ��ѿ��ӷ�ƨ
    return resTokenFlow;
}

bool Parser::isClassName(string name) {
    for (auto i : m_classNames) {
        if (i == name)
            return true;
    }
    return false;
}

bool Parser::isFuncName(string targetStr) {
    string hFile = m_curFileName.substr(0, m_curFileName.size() - 3) + "h";
    for (auto i : m_hTokenFlows[hFile]) {
        if (i.getTokenStr() == targetStr) { //�������������ƣ����Ͻ�׳��
            return true;
        }
    }
    return false;
}