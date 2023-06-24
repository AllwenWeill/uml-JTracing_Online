#include "Parser.h"
Parser::Parser(unordered_map<string, vector<Token>> hTokenFlows, unordered_map<string, vector<Token>> cppTokenFlows, vector<Token> tokenVector, vector<string> classNames, ClassList* pCList, string curFileName, string startClassName)
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

void Parser::mainParser() {
    int tmpSize = m_tokenVector.size() - 1;
    while(m_offset <= tmpSize && m_tokenVector.size() != 0) { //warning:此处m_offset是无符号类型，与int类型比较会使tmpSize也统一为无符号类型，因此如果tmpSize<0时会转换成最大数，故需要加一步判断
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
            if (Type_uset.count(curTokenKind)) { //如果在Type表中，则说明当前token为int等类型关键字
                getNextToken(); //eat type
                handlFunc();
                
            }
            if (curTokenKind == TokenKind::EndOfFile) {
                m_offset = m_tokenVector.size();
                break;
            }
            if (m_offset >= tmpSize) {
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

std::shared_ptr<ExprAST> Parser::parsePrimary() { //解析初级表达式
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

/*std::shared_ptr<ObjCallFuncPAST> Parser::ParseObjCallFuncP() {

}

std::shared_ptr<ObjCallFuncAAST> Parser::ParseObjCallFuncA() {

}
*/
std::shared_ptr<ExprAST> Parser::ParseBitWide() {
    getNextToken();
    if (curTokenKind != TokenKind::IntegerLiteral) { //[]内第一个Token期待的是数字类型的常数
        LE.addnote("reference to non-constant variable is not allowed in a constant expression", curToken.TL.m_tokenLine);
        return nullptr;
    }
    auto numWide = ParseNumber(); 
    if (curTokenKind != TokenKind::Colon) { //此时期待一个:
        LE.addnote("packed dimensions require a full range specification", curToken.TL.m_tokenLine);
        return nullptr;
    }
    getNextToken();
    if (curTokenKind != TokenKind::IntegerLiteral) { //[]内最后一个Token期待的是数字类型的常数
        LE.addnote("reference to non-constant variable is not allowed in a constant expression", curToken.TL.m_tokenLine);
        return nullptr;
    }
    auto numRange = ParseNumber();
    if (curTokenKind != TokenKind::CloseBracket) { //此时期待一个]
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

// std::shared_ptr<PrototypeAST> Parser::ParseModulePrototype(){ //解析module声明（可能不需要？）
//     getNextToken();
//     if(curTokenKind != TokenKind::Identifier){ //module需要有module名
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

std::shared_ptr<DefinitionAST> Parser::ParseModuleDefinition() { //解析module实现
    getNextToken();
    if (curTokenKind != TokenKind::Identifier) { //module需要有module名
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
    getNextToken(); //eat Always_ff关键字
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
    getNextToken(); //eat Always_comb关键字
    auto exprs = parsePrimary();
    return std::make_shared<Always_combAST>(exprs);
}

std::shared_ptr<ForAST> Parser::ParseFor() {
    getNextToken(); //eat for关键字
    LoopInformation LP;
    if (curTokenKind != TokenKind::OpenParenthesis) { //此时期待一个(
        LE.addnote("expected '('", curToken.TL.m_tokenLine);
        return nullptr;
    }
    getNextToken(); //eat (
    auto init = ParseExpression();
    if (curTokenKind != TokenKind::Semicolon) {
        LE.addnote("expected ';'", curToken.TL.m_tokenLine);
    }
    getNextToken(); //eat ;
    int conditionLenth = 0;
    string loopCondition;
    while (m_tokenVector[m_offset + conditionLenth].getTokenKind() != TokenKind::Semicolon) {
        loopCondition = loopCondition + m_tokenVector[m_offset + conditionLenth].getTokenStr();
        ++conditionLenth;
    }
    auto LHS = ParseIdentifierExpr(TokenKind::NullKeyword);
    auto cmp = ParseCmpOpRHS(LHS);
    if (curTokenKind != TokenKind::Semicolon) {
        LE.addnote("expected ';'", curToken.TL.m_tokenLine);
    }
    LP.LoopCondition = loopCondition;
    getNextToken(); //eat ;
    auto step = ParseExpression();
    if (curTokenKind != TokenKind::CloseParenthesis) {
        LE.addnote("expected ')'", curToken.TL.m_tokenLine);
        return nullptr;
    }
    getNextToken(); //eat )
    auto expr = ParseExpression();
    if (curTokenKind != TokenKind::OpenBrace) { 
        return nullptr;
    }
    getNextToken(); //eat '{'
    vector<shared_ptr<ExprAST>> exprs;

    int start = m_pCList->getFuncCallInfo().size();
    while (curTokenKind != TokenKind::CloseBrace) {
        auto expr = ParseExpression();
        exprs.push_back(expr);
        if (curTokenKind == TokenKind::Semicolon) {
            getNextToken(); //eat ';'
        }
    }
    int end = m_pCList->getFuncCallInfo().size();
    for (int i = start; i < end; i++) {
        LP.timeLine.push_back(i);
        LP.loopIcludeClassName.push_back(m_pCList->getFuncCallInfo().at(i).callClassName);
        LP.loopIcludeClassName.push_back(m_pCList->getFuncCallInfo().at(i).invokeClassName);
    }
    m_pCList->addLoopInfo(LP);
    getNextToken(); //eat '}'
    return std::make_shared<ForAST>(expr, init, cmp, step);
}

/*std::shared_ptr<ExprAST> Parser::HandleAlt() {
    return 
}*/

std::shared_ptr<InitialAST> Parser::ParseInitial() {
    getNextToken(); //eat Initial关键字
    shared_ptr<ExprAST> expr = nullptr;
    if (curTokenKind != TokenKind::BeginKeyword) { //则说明仅有单行表达式
        expr = parsePrimary();
    }
    else { //则说明有多行表达式
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
    vector<shared_ptr<ExprAST>> exprs;
    int conditionLenth = 1;
    string altCondition;
    while (m_tokenVector[m_offset + conditionLenth].getTokenKind() != TokenKind::CloseParenthesis) {
        altCondition = altCondition + m_tokenVector[m_offset + conditionLenth].getTokenStr();
        ++conditionLenth;
    }
    auto cond = ParseParenExpr();
    AltInformation AT;
    AT.altCondition = altCondition;
    int start = m_pCList->getFuncCallInfo().size();
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
    int end = m_pCList->getFuncCallInfo().size();
    for (int i = start; i < end; i++) {
        AT.timeLine.push_back(i + 1);
        AT.altIncludeClassName.push_back(m_pCList->getFuncCallInfo().at(i + 1).callClassName);
        AT.altIncludeClassName.push_back(m_pCList->getFuncCallInfo().at(i + 1).invokeClassName);
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
    string elseCondition = "NULLCONDITION";
    if (curTokenKind == TokenKind::IfKeyword) {
        elseCondition.clear();
        if (curTokenKind != TokenKind::OpenParenthesis) {
            LE.addnote("expected expression", curToken.TL.m_tokenLine);
            return nullptr;
        }
        int conditionLenth = 1;
        while (m_tokenVector[m_offset + conditionLenth].getTokenKind() != TokenKind::CloseParenthesis) {
            elseCondition = elseCondition + m_tokenVector[m_offset + conditionLenth].getTokenStr();
            ++conditionLenth;
        }
        auto cond = ParseParenExpr();
    }
    vector<shared_ptr<ExprAST>> exprs;
    int start = m_pCList->getFuncCallInfo().size();
    if (curTokenKind != TokenKind::OpenBrace) {
        auto expr = ParseExpression();
        exprs.push_back(expr);
    }
    while (curTokenKind != TokenKind::CloseBrace) {
        auto expr = ParseExpression();
        exprs.push_back(expr);
    }
    int end = m_pCList->getFuncCallInfo().size();
    for (int i = start; i < end; i++) {
        string a;
        a = m_pCList->getFuncCallInfo().at(i).invokeClassName;
        AT.timeLine.push_back(i + 1);
        AT.altIncludeClassName.push_back(m_pCList->getFuncCallInfo().at(i + 1).callClassName);
        AT.altIncludeClassName.push_back(m_pCList->getFuncCallInfo().at(i + 1).invokeClassName);
    }
    int elseStartPosition = start + 1;
    m_pCList->modifyAltInfo(AT, elseStartPosition, elseCondition);
    return std::move(std::make_shared<ElseAST>(cond, exprs));
}

std::shared_ptr<ExprAST> Parser::ParseParenExpr() { //不可适用于for()
    getNextToken(); // eat (.
    shared_ptr<ExprAST> V = nullptr;
    switch (curTokenKind) {
    case TokenKind::Identifier: {
        if (!VariableInfo_umap.count(curToken.getTokenStr())) { //如果该标识符不存在，则说明调用未定义标识符
            string tmpStr = "use of undeclared identifier '";
            tmpStr += curToken.getTokenStr();
            tmpStr += "'";
            LE.addnote(tmpStr, curToken.TL.m_tokenLine);
            return nullptr;
        }
        auto LHS = ParseIdentifierExpr(TokenKind::NullKeyword);
        if (curTokenKind != TokenKind::CloseParenthesis) { //如果不为)则说明后面仍需判断
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
        if (!VariableInfo_umap.count(curToken.getTokenStr())) { //如果该标识符不存在，则说明调用未定义标识符
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
        if (VariableInfo_umap.count(IdName)) { //如果该标识符已经存在，则说明重复定义
            LE.addnote("previous definition here", curToken.TL.m_tokenLine);
            return nullptr;
        }
        //如果为首次定义，则刷新VF结构体内的变量信息，并加入变量表
        VF.name = IdName;
        VF.kind = TokenKindtoString(varType);
        VariableInfo_umap[IdName] = VF;
        variableTypeFlag = TokenKind::NullKeyword; //将标识符flag还原
        break;
    }
    case TokenKind::NullKeyword: {//说明非定义变量，该标识符被调用
        TokenKind nextTokenKind = m_tokenVector[m_offset+1].getTokenKind();
        //TokenKind n_nextTokenKind = m_tokenVector[m_offset + 1].getTokenKind();
        if (isClassName(curToken.getTokenStr()) || nextTokenKind == TokenKind::OpenParenthesis || nextTokenKind == TokenKind::Star || nextTokenKind == TokenKind::Dot || nextTokenKind == TokenKind::MemberPointerAccess) { //当前为一个函数
            auto V = handlObj(); //暂时不用return 
            //getNextToken(); //eat
            return V; //不能返回一个空
        }
        else if (!VariableInfo_umap.count(IdName) && nextTokenKind != TokenKind::OpenParenthesis) { //如果该标识符不存在，则说明调用未定义标识符
            string tmpStr = "use of undeclared identifier '";
            tmpStr += IdName;
            tmpStr += "'";
            LE.addnote(tmpStr, curToken.TL.m_tokenLine);
            return nullptr;
        }
        
        break;
    }
    default: //不符合定义类型的关键字
        LE.addnote("invaild type", curToken.TL.m_tokenLine);
        return nullptr;
        break;
    }
    getNextToken();
    auto V = std::make_shared<VariableExprAST>(IdName, VariableInfo_umap[IdName].kind); //需要判断后面是否为;号?
    return std::move(V);
}

std::shared_ptr<ExprAST> Parser::ParseExpression() {
    auto LHS = parsePrimary();
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

std::shared_ptr<ExprAST> Parser::ParseBinOpRHS(int ExprPrec, std::shared_ptr<ExprAST> LHS) { //解析二元表达式的右半部分
    if (curTokenKind == TokenKind::Semicolon)
        return nullptr;
    while (1) {
        int curTokenPrec = GetTokPrecedence(); //获取当前Token运算符的优先级
        if (curTokenPrec < ExprPrec)
            return LHS;
        string BinOp = curToken.getTokenStr();
        auto RHS = parsePrimary();
        if (!RHS)
            return nullptr;
        LogP.addnote("->parsing a Binary Expression...");
        int nextOpPrec = GetTokPrecedence(); //获取下一个运算符的优先级
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
    else if (curStr != "+" //增加对curTokenKind的判断，增强函数健壮性
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
    return ParseModuleDefinition(); //此处无法明确到底是解析module声明还是定义？
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
    string a = curToken1.getTokenStr();
    string b = TokenKindtoString(curTokenKind1);
    getNextToken(); //eat Identifier
    if (curTokenKind != TokenKind::OpenParenthesis) { //如果此时下一个Token是'('，则说明此时是一个函数
        --m_offset;
        curToken = m_tokenVector[--m_offset];
        curTokenKind = curToken.getTokenKind();
        cout << "-->Not a Funciton..." << endl;;
        return;
    }
    while (curTokenKind != TokenKind::CloseParenthesis) { //跳过func()括号中参数，对于生成uml时序图意义不大(暂时不考虑参数设计函数调用情况)
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
        FC.invokeClassName = m_curFileName.substr(0, m_curFileName.size() - 4);
        FC.callClassName = m_startClassName;
        FC.FuncName = curToken.getTokenStr();
        string funcname = FC.FuncName;
        int parentoffset = 1;
        if (m_tokenVector[m_offset + parentoffset].getTokenKind() == TokenKind::OpenParenthesis) {
            while (m_tokenVector[m_offset + parentoffset].getTokenKind() != TokenKind::CloseParenthesis) {
                funcname = funcname + m_tokenVector[m_offset + parentoffset].getTokenStr();
                ++parentoffset;
            }
            funcname = funcname + " ";
            funcname = funcname + m_tokenVector[m_offset + parentoffset].getTokenStr();
            FC.FuncName = funcname;
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
        Parser dfsPar(m_hTokenFlows, m_cppTokenFlows, targetTokenFlows, m_classNames, m_pCList, FC.invokeClassName + ".cpp", m_startClassName);
        int afterDfsCallOrder = (m_pCList->getFuncCallInfo()).size();
        //统计递归次数
        unordered_map<int, int> tmpCurDescendantsSequenceMap;
        for (int i = curFuncCallOrder + 1; i <= afterDfsCallOrder; i++) {
            tmpCurDescendantsSequenceMap[i] = i;
        }
        //从子孙节点中提纯子节点
        m_pCList->modifyDescendantsSequence(curFuncCallOrder, tmpCurDescendantsSequenceMap);
        m_pCList->modifyDirectDescendantsSequence(curFuncCallOrder, tmpCurDescendantsSequenceMap);
        for (int i = curFuncCallOrder + 1; i <= afterDfsCallOrder; i++) {
            auto tmpDescendantsSequenceMap = (m_pCList->getFuncCallInfo()).at(i).descendantsSequence;
            for (auto j : tmpDescendantsSequenceMap) {
                m_pCList->DeleteDirectDDescendantsSequenceEnum(curFuncCallOrder, j.first);
            }
        }
        auto tmpDescendantsSequenceMapFetch = (m_pCList->getFuncCallInfo()).at(curFuncCallOrder).directDescendantsSequence;
        //将子节点的callClassName设置为当前类名
        for (auto i = tmpDescendantsSequenceMapFetch.begin(); i != tmpDescendantsSequenceMapFetch.end(); ++i) {
            m_pCList->setCallClassName(i->first, curFuncCallOrder);
        }
        //装载激活相关信息
        auto activationClassName = m_pCList->getFuncCallInfo().at(curFuncCallOrder).invokeClassName;
        m_pCList->modifyClassActivationInfo(activationClassName, curFuncCallOrder, 1);
        for (int i = curFuncCallOrder + 1; i <= afterDfsCallOrder; i++) {
            if (m_pCList->getActivationInfo().at(activationClassName).find(i) == m_pCList->getActivationInfo().at(activationClassName).end()) {
                m_pCList->modifyClassActivationInfo(activationClassName, i, 1);
            }
            else {
                auto tmpDescendantsSequenceMap = (m_pCList->getFuncCallInfo()).at(i).descendantsSequence;
                for (auto j : tmpDescendantsSequenceMap) {
                    int k = m_pCList->getActivationInfo().at(activationClassName).at(j.first) + 1;
                    m_pCList->modifyClassActivationInfo(activationClassName, i, k);
                }
            }
        }
        // vector<Token> targetTokenFlows2 = filterTokenFlow(FC.FuncName, FC.invokeClassName+".cpp");
        // Parser (m_hTokenFlows, m_cppTokenFlows, targetTokenFlows2, m_classNames, m_pCList, FC.invokeClassName + ".cpp");
        return make_shared<FuncAST>(FC.FuncName);
    }
    cout << curToken.getTokenStr();
    Token nextToken = m_tokenVector[m_offset + 1];
    TokenKind nextTokenKind = nextToken.getTokenKind();
    Token n_nextToken = m_tokenVector[m_offset + 2]; //next��next
    TokenKind n_nextTokenKind = n_nextToken.getTokenKind();
    //case1: A a();
    if (nextTokenKind == TokenKind::Identifier) {
        ObjInstantiation_umap[curToken.getTokenStr()].emplace_back(nextToken.getTokenStr()); //add ObjInstantiation_umap
        while (curTokenKind != TokenKind::Semicolon) {
            cout << curToken.getTokenStr() + "---------";
            getNextToken();
        }
        cout << "next";
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
        //通过a来找爹
        FuncCallInformation FC;
        FC.invokeClassName = findClassName(curToken.getTokenStr());
        getNextToken(); //eat Indentifier,like 'a'
        getNextToken(); //eat Dot;
        FC.FuncName = curToken.getTokenStr();
        string funcname = FC.FuncName;
        int parentoffset = 1;
        if (m_tokenVector[m_offset + parentoffset].getTokenKind() == TokenKind::OpenParenthesis) {
            while (m_tokenVector[m_offset + parentoffset].getTokenKind() != TokenKind::CloseParenthesis) {
                funcname = funcname + m_tokenVector[m_offset + parentoffset].getTokenStr();
                ++parentoffset;
            }
            funcname = funcname + " ";
            funcname = funcname + m_tokenVector[m_offset + parentoffset].getTokenStr();
            FC.FuncName = funcname;
        }
        FC.callClassName = m_startClassName;
        m_pCList->addFuncCallInfo(FC);

        while (curTokenKind != TokenKind::Semicolon) {
            getNextToken();
        }
        //getNextToken(); //eat ;
        cout << "parseing obj call function..." << endl;
        cout << "---->" << FC.invokeClassName << "." << FC.FuncName << "()" << endl;
        int curFuncCallOrder = (m_pCList->getFuncCallInfo()).size();
        vector<Token> targetTokenFlows = filterTokenFlow(FC.FuncName, FC.invokeClassName + ".cpp");
        Parser dfs(m_hTokenFlows, m_cppTokenFlows, targetTokenFlows, m_classNames, m_pCList, FC.invokeClassName + ".cpp", m_startClassName);
        int afterDfsCallOrder = (m_pCList->getFuncCallInfo()).size();
        //统计递归次数
        unordered_map<int, int> tmpCurDescendantsSequenceMap;
        for (int i = curFuncCallOrder + 1; i <= afterDfsCallOrder; i++) {
            tmpCurDescendantsSequenceMap[i] = i;
        }
        //从子孙节点中提纯子节点
        m_pCList->modifyDescendantsSequence(curFuncCallOrder, tmpCurDescendantsSequenceMap);
        m_pCList->modifyDirectDescendantsSequence(curFuncCallOrder, tmpCurDescendantsSequenceMap);
        for (int i = curFuncCallOrder + 1; i <= afterDfsCallOrder; i++) {
            auto tmpDescendantsSequenceMap = (m_pCList->getFuncCallInfo()).at(i).descendantsSequence;
            for (auto j : tmpDescendantsSequenceMap) {
                m_pCList->DeleteDirectDDescendantsSequenceEnum(curFuncCallOrder, j.first);
            }
        }
        auto tmpDescendantsSequenceMapFetch = (m_pCList->getFuncCallInfo()).at(curFuncCallOrder).directDescendantsSequence;
        //将子节点的callClassName设置为当前类名
        for (auto i = tmpDescendantsSequenceMapFetch.begin(); i != tmpDescendantsSequenceMapFetch.end(); ++i) {
            m_pCList->setCallClassName(i->first, curFuncCallOrder);
        }
        //装载激活相关信息
        auto activationClassName = m_pCList->getFuncCallInfo().at(curFuncCallOrder).invokeClassName;
        m_pCList->modifyClassActivationInfo(activationClassName, curFuncCallOrder, 1);
        for (int i = curFuncCallOrder + 1; i <= afterDfsCallOrder; i++) {
            if (m_pCList->getActivationInfo().at(activationClassName).find(i) == m_pCList->getActivationInfo().at(activationClassName).end()) {
                m_pCList->modifyClassActivationInfo(activationClassName, i, 1);
            }
            else {
                auto tmpDescendantsSequenceMap = (m_pCList->getFuncCallInfo()).at(i).descendantsSequence;
                for (auto j : tmpDescendantsSequenceMap) {
                    int k = m_pCList->getActivationInfo().at(activationClassName).at(j.first) + 1;
                    m_pCList->modifyClassActivationInfo(activationClassName, i, k);
                }
            }
        }
        // vector<Token> targetTokenFlows2 = filterTokenFlow(FC.FuncName, FC.invokeClassName+".cpp");
        // Parser (m_hTokenFlows, m_cppTokenFlows, targetTokenFlows2, m_classNames, m_pCList, FC.invokeClassName + ".cpp");
        return make_shared<FuncAST>(FC.FuncName);
    }
    //case4: a->work();
    else if (nextTokenKind == TokenKind::MemberPointerAccess) {
        //通过a来找爹
        FuncCallInformation FC;
        FC.invokeClassName = findClassName(curToken.getTokenStr());
        getNextToken(); //eat Indentifier,like 'a'
        getNextToken(); //eat '->'
        FC.FuncName = curToken.getTokenStr();
        string funcname = FC.FuncName;
        Token nextTokens = m_tokenVector[m_offset + 1];
        int parentoffset = 1;
        if (m_tokenVector[m_offset + parentoffset].getTokenKind() == TokenKind::OpenParenthesis) {
            // funcname = funcname +" ";
            while (m_tokenVector[m_offset + parentoffset].getTokenKind() != TokenKind::CloseParenthesis) {
                funcname = funcname + m_tokenVector[m_offset + parentoffset].getTokenStr();
                ++parentoffset;
            }
            funcname = funcname + " ";
            funcname = funcname + m_tokenVector[m_offset + parentoffset].getTokenStr();
            FC.FuncName = funcname;
        }
        FC.callClassName = m_startClassName;
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
        Parser dfs(m_hTokenFlows, m_cppTokenFlows, targetTokenFlows, m_classNames, m_pCList, FC.invokeClassName + ".cpp", m_startClassName);
        int afterDfsCallOrder = (m_pCList->getFuncCallInfo()).size();
        //统计递归次数
        unordered_map<int, int> tmpCurDescendantsSequenceMap;
        for (int i = curFuncCallOrder + 1; i <= afterDfsCallOrder; i++) {
            tmpCurDescendantsSequenceMap[i] = i;
        }
        //从子孙节点中提纯子节点
        m_pCList->modifyDescendantsSequence(curFuncCallOrder, tmpCurDescendantsSequenceMap);
        m_pCList->modifyDirectDescendantsSequence(curFuncCallOrder, tmpCurDescendantsSequenceMap);
        for (int i = curFuncCallOrder + 1; i <= afterDfsCallOrder; i++) {
            auto tmpDescendantsSequenceMap = (m_pCList->getFuncCallInfo()).at(i).descendantsSequence;
            for (auto j : tmpDescendantsSequenceMap) {
                m_pCList->DeleteDirectDDescendantsSequenceEnum(curFuncCallOrder, j.first);
            }
        }
        auto tmpDescendantsSequenceMapFetch = (m_pCList->getFuncCallInfo()).at(curFuncCallOrder).directDescendantsSequence;
        //将子节点的callClassName设置为当前类名
        for (auto i = tmpDescendantsSequenceMapFetch.begin(); i != tmpDescendantsSequenceMapFetch.end(); ++i) {
            m_pCList->setCallClassName(i->first, curFuncCallOrder);
        }
        if (!((m_pCList->getFuncCallInfo()).at(curFuncCallOrder).callClassName.size())) {
            (m_pCList->getFuncCallInfo()).at(curFuncCallOrder).callClassName = "Main";
        }
        //装载激活相关信息
        auto activationClassName = m_pCList->getFuncCallInfo().at(curFuncCallOrder).invokeClassName;
        m_pCList->modifyClassActivationInfo(activationClassName, curFuncCallOrder, 1);
        for (int i = curFuncCallOrder + 1; i <= afterDfsCallOrder; i++) {
            if (m_pCList->getActivationInfo().at(activationClassName).find(i) == m_pCList->getActivationInfo().at(activationClassName).end()) {
                m_pCList->modifyClassActivationInfo(activationClassName, i, 1);
            }
            else {
                auto tmpDescendantsSequenceMap = (m_pCList->getFuncCallInfo()).at(i).descendantsSequence;
                for (auto j : tmpDescendantsSequenceMap) {
                    int k = m_pCList->getActivationInfo().at(activationClassName).at(j.first) + 1;
                    m_pCList->modifyClassActivationInfo(activationClassName, i, k);
                }
            }
        }

        if (!((m_pCList->getFuncCallInfo()).at(curFuncCallOrder).callClassName.size())) {
            (m_pCList->getFuncCallInfo()).at(curFuncCallOrder).callClassName = "Main";
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
    if (curTokenKind != TokenKind::OpenParenthesis) { //while格式残缺，expected '('
        return nullptr;
    }
    int conditionLenth = 1;
    string loopCondition;
    while (m_tokenVector[m_offset + conditionLenth].getTokenKind() != TokenKind::CloseParenthesis) {
        loopCondition = loopCondition + m_tokenVector[m_offset + conditionLenth].getTokenStr();
        ++conditionLenth;
    }
    getNextToken(); //eat '('
    auto LHS = ParseIdentifierExpr(TokenKind::NullKeyword);
    auto cmp = ParseCmpOpRHS(LHS);
    getNextToken(); //eat ')'
    vector<shared_ptr<ExprAST>> exprs;
    LoopInformation LP;
    LP.LoopCondition = loopCondition;
    int start = m_pCList->getFuncCallInfo().size();
    getNextToken(); //eat '{'

    while (curTokenKind != TokenKind::CloseBrace) {
        auto expr = ParseExpression();
        exprs.push_back(expr);
        if (curTokenKind == TokenKind::Semicolon) {
            getNextToken(); //eat ';'
        }
    }
    int end = m_pCList->getFuncCallInfo().size();
    for (int i = start; i < end; i++) {
        LP.timeLine.push_back(i);
        LP.loopIcludeClassName.push_back(m_pCList->getFuncCallInfo().at(i).callClassName);
        LP.loopIcludeClassName.push_back(m_pCList->getFuncCallInfo().at(i).invokeClassName);
    }
    m_pCList->addLoopInfo(LP);
    getNextToken(); //eat '}'
    return make_shared<WhileAST>(cmp, exprs);
}
/*
std::shared_ptr<WhileAST>  Parser::ParseDoWhile() {

}*/

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
    LogP.addnote("parsed Include...");
    getNextToken(); //eat include;
    if (curTokenKind == TokenKind::LessThan); {
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
function: 找到a.work()中a的class的.cpp文件词法分析后的tokenflow，筛选出其中work()的token
param: 目标函数名，目标.cpp文件名
return: 筛选得到的tokenflow
*/
vector<Token> Parser::filterTokenFlow(string targetFuncName, string targetfFleName) {
    vector<Token> MresTokenFlow;
    vector<Token> resTokenFlow;
    for (int i = 0; i < m_cppTokenFlows[targetfFleName].size(); i++) { //遍历扫描该.cpp文件的整个tokenflow
        Token nextFourToken;
        Token curToken = m_cppTokenFlows[targetfFleName][i];
        if (i <= m_cppTokenFlows[targetfFleName].size() - 4) {
            nextFourToken = m_cppTokenFlows[targetfFleName][i+4];
        }
        //vector<Token> resTokenFlow;
        if (Type_uset.count(curToken.getTokenKind()) && nextFourToken.getTokenStr() == targetFuncName) { //找到目标函数token位置
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
    resTokenFlow = MresTokenFlow; //由于编译时符号表可能出现了问题产生bug，故此脱裤子放屁
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
        if (i.getTokenStr() == targetStr) { //可增加条件限制，保障健壮性
            return true;
        }
    }
    return false;
}