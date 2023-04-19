#include "Lexer.h"
Lexer::Lexer(const string *psm, unsigned long int indexOffset)
    :m_psm(psm),
    init_psm(psm),
    m_indexOffset(indexOffset)
{
    offset_count = 0;
    lineNum = 1;
    keywoedsCount = 0;
    scanText();
}
//function: 扫描阶段： 主要负责完成一些不需要生成词法单元的简单处理，比如删除注释和将多个连续的空白字符压缩成一个字符，然后产生词素流
void Lexer::scanText(){
    string tempStr;
    cout<<*m_psm<<endl;
    while(!islastChar()){
        char tempCh = (*m_psm).at(offset_count);
        //cout<<"tempCh:"<<tempCh<<endl;
        // cout<<"offset:"<<offset_count<<endl;
        switch (tempCh){
            case ' ':
                keywoedsCount++;
                keywords.push_back(tempStr);
                tempStr.clear();
                advance();
                break;
            case '\t':
            case '\v':
            case '\f':
            case '\0':
            case '\r':
            case '\n': //表示换行
                lineNum++;
                keywoedsCount++;
                keywords.push_back(tempStr);
                tempStr.clear();
                advance();
                break;
            case '/':
                advance();
                tempCh = (*m_psm).at(offset_count);
                switch (tempCh)
                {
                case '*':   //块注释
                    scanBlockComment();
                    break;
                case '/':   //行注释
                    scanLineComment();
                    break;
                default:  //说明仅有"/"，需要产生error信息
                    break;
                }
                break;
            default:  //说明为普通文字字符，而非空格注释等trivial things
                if(isChar(tempCh)){ //如果是字母（关键字，变量名称）
                    scanLetter(); //连续扫描当前字母串（此处待完善，变量名可以_下划线开头）
                    break;
                }
                else if(tempCh == '"'){ //如果是双引号，则为字符串文本
                    scanString(); //连续扫描字符串
                    advance();
                }
                else if(isNum(tempCh)){ //如果是数字
                    scanNumber(); //连续扫描当前数字串，注意区分小数点
                    break;
                }
                else if(isOperator(tempCh)){ //如果是运算符
                    switch(tempCh)
                    {
                        keywords.push_back(tempStr);
                        tempStr.clear();
                        tempStr.push_back(tempCh);
                        keywords.push_back(tempStr);
                        tempStr.clear(); //以上三行很冗余，即先将char转string再压入容器
                    case '+': {
                        char nextCh = (*m_psm).at(offset_count + 1);
                        if (nextCh == '+') {
                            tokenVector.push_back(create(TokenKind::DoublePlus, lineNum, keywords.size() - 1, "++"));
                            advance();
                        }
                        else {
                            tokenVector.push_back(create(TokenKind::Plus, lineNum, keywords.size() - 1, "+"));
                        }
                        advance();
                        break;
                    }
                    case '-': {
                        char nextCh = (*m_psm).at(offset_count + 1);
                        if (nextCh == '-') {
                            tokenVector.push_back(create(TokenKind::DoubleMinus, lineNum, keywords.size() - 1, "--"));
                            advance();
                        }
                        else {
                            tokenVector.push_back(create(TokenKind::Minus, lineNum, keywords.size() - 1, "-"));
                        }
                        advance();
                        break; 
                    }
                        case '*':
                            tokenVector.push_back(create(TokenKind::Star, lineNum, keywords.size()-1, "*"));
                            advance();
                            break;
                        case '/':
                            tokenVector.push_back(create(TokenKind::Slash, lineNum, keywords.size()-1, "/"));
                            advance();
                            break;
                        case '%':
                            tokenVector.push_back(create(TokenKind::Percent, lineNum, keywords.size()-1, "%"));
                            advance();
                            break;
                        case '|':
                            tokenVector.push_back(create(TokenKind::Or, lineNum, keywords.size()-1, "|"));
                            advance();
                            break;
                        case '&':
                            tokenVector.push_back(create(TokenKind::And, lineNum, keywords.size()-1, "&"));
                            advance();
                            break;
                        default:
                            perror("isOperator:");
                            exit(-1);
                    }
                }
                else if(isCmpOperator(tempCh)){ //如果是比较符
                    keywords.push_back(tempStr);
                    tempStr.clear();
                    tempStr.push_back(tempCh);
                    advance();
                    char nextCh = (*m_psm).at(offset_count);
                    switch (tempCh)
                    {
                    case '=':
                        if(nextCh == '='){ //==
                            tokenVector.push_back(create(TokenKind::DoubleEquals, lineNum, keywords.size()-1, "=="));
                            tempStr.push_back(nextCh);
                            keywords.push_back(tempStr);
                            tempStr.clear();
                            advance();
                            break;
                        }
                        else if(nextCh == ' '){ //说明只有一个等号=
                            keywords.push_back(tempStr);
                            tokenVector.push_back(create(TokenKind::Equals, lineNum, keywords.size()-1, "="));
                            tempStr.clear();
                            break;
                        }
                        else{ //待完善

                        }
                    case '<':
                        if(nextCh == '='){ //<=
                            tokenVector.push_back(create(TokenKind::LessThanEquals, lineNum, keywords.size()-1, "<="));
                            tempStr.push_back(nextCh);
                            keywords.push_back(tempStr);
                            tempStr.clear();
                            advance();
                            break;
                        }
                        else if(nextCh == ' '){
                            keywords.push_back(tempStr);
                            tempStr.clear();
                            break;
                        }
                        else{ //可能是赋值语句，可还有单纯的>和<比较情况
                            tokenVector.push_back(create(TokenKind::LessThan, lineNum, keywords.size()-1, "<"));
                            keywords.push_back(tempStr);
                            tempStr.clear();
                            advance();
                            break;
                        }
                    case '>':
                        if(nextCh == '='){ //<=
                            tokenVector.push_back(create(TokenKind::GreaterThanEquals, lineNum, keywords.size()-1, ">="));
                            tempStr.push_back(nextCh);
                            keywords.push_back(tempStr);
                            tempStr.clear();
                            advance();
                            break;
                        }
                        else if(nextCh == ' '){
                            keywords.push_back(tempStr);
                            tempStr.clear();
                            break;
                        }
                        else{ //可能是赋值语句，可还有单纯的>和<比较情况
                            tokenVector.push_back(create(TokenKind::GreaterThan, lineNum, keywords.size()-1, ">"));
                            keywords.push_back(tempStr);
                            tempStr.clear();
                            advance();
                            break;
                        }
                    default: 
                        break;
                    }
                }
                else if(tempCh == ';'){
                    tokenVector.push_back(create(TokenKind::Semicolon, lineNum, keywords.size()-1, ";"));
                    advance();
                }
                else if (tempCh == ':') {
                    tokenVector.push_back(create(TokenKind::Colon, lineNum, keywords.size() - 1, ":"));
                    advance();
                }
                else if(tempCh == '('){ //如果括号与文本中无空格相连，则无法检测到
                    tokenVector.push_back(create(TokenKind::OpenParenthesis, lineNum, keywords.size()-1, "("));
                    advance();
                }
                else if(tempCh == ')'){
                    tokenVector.push_back(create(TokenKind::CloseParenthesis, lineNum, keywords.size()-1, ")"));
                    advance();
                }
                else if (tempCh == '[') {
                    tokenVector.push_back(create(TokenKind::OpenBracket, lineNum, keywords.size() - 1, "["));
                    advance();
                }
                else if (tempCh == ']') {
                    tokenVector.push_back(create(TokenKind::CloseBracket, lineNum, keywords.size() - 1, "]"));
                    advance();
                }
                else if (tempCh == '{') {
                    tokenVector.push_back(create(TokenKind::OpenBrace, lineNum, keywords.size() - 1, "{"));
                    advance();
                }
                else if (tempCh == '}') {
                    tokenVector.push_back(create(TokenKind::CloseBrace, lineNum, keywords.size() - 1, "}"));
                    advance();
                }
                else if (tempCh == '$') {
                    tokenVector.push_back(create(TokenKind::Dollar, lineNum, keywords.size() - 1, "$"));
                    advance();
                }
                else if (tempCh == '@') {
                    tokenVector.push_back(create(TokenKind::At, lineNum, keywords.size() - 1, "@"));
                    advance();
                }
                else{
                    advance();
                }
        }
    }
    cout<<"test:打印keywords所有元素"<<endl;
    for(string str : keywords){
        // for(auto ch : str)
        //     printf("%x ", ch);
        cout<<str<<" "<<endl;
    }
    cout<<"TokenKind:------------"<<endl;
    for(auto k : tokenVector){
        cout<<k.getTokenStr()<<":"<<k.getTokenKindStr()<<"行号:"<<k.TL.m_tokenLine<<endl;
    }
}
void Lexer::advance(){
    offset_count++;
}
void Lexer::advance(int count){
    offset_count += count;
}
bool Lexer::islastChar(){//用'/0'也可以判断，但是不够
    if(offset_count >= m_indexOffset){
        tokenVector.push_back(create(TokenKind::EndOfFile, lineNum, keywords.size()-1, "EOF"));
        return true;
    }
    else{
        return false;
    }
}
void Lexer::scanBlockComment(){
    while(true){
        char tempCh = (*m_psm).at(offset_count);
        if (tempCh == '*' && (*m_psm).at(offset_count+1) == '/') { //此处offset_count+1有越界风险
            advance(2);
            break;
        } 
        else if(islastChar()){
            break;
        }
        else {
            advance();
        }
    }
}
void Lexer::scanLineComment(){
    while (true) {
        char tempCh = (*m_psm).at(offset_count);
        if (tempCh == 0x0a) {//0x0a表示换行
            lineNum++;
            advance(1);
            break;
        }
        else if(islastChar()){
            break;
        }
        else{
            advance();
        }
    }
}
Token Lexer::create(TokenKind kind, int tokenLine, int tokenNum, string tokenString){
    return Token(kind, tokenNum, tokenLine, tokenString);
}
Lexer::~Lexer(){
    
}
bool isChar(const char &ch){
    if((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || ch == '_' || ch == '.')
        return true;
    else
        return false;
}
bool isNum(const char &ch){
    if(ch >= '0' && ch <= '9')
        return true;
    else
        return false;
}
bool isOperator(const char &ch){
    switch (ch)
    {
        case '*':
        case '%':
        case '/':
        case '&':
        case '+':
        case '-':
        case '|':
            return true;
        default:
            return false;
    }
}
bool isCmpOperator(const char &ch){
    switch(ch)
    {
        case '<':
        case '=':
        case '>':
            return true;
        default:
            return false;
    }
}
void Lexer::scanLetter(){
    string tmpStr;
    tmpStr.push_back((*m_psm).at(offset_count));
    while(true){
        advance();
        char tempCh = (*m_psm).at(offset_count);
        //if(tempCh == '(') //可能需要加扫描D(x)这种情况
        if(tempCh == ' ' || tempCh == 0x0a || !isChar(tempCh)){ //如果遇到空格或者换行
            keywords.push_back(tmpStr);
            TokenKind kind;
            if(lookupKeyword(tmpStr, kind)){ //说明是关键字
                tokenVector.push_back(create(kind, lineNum, keywords.size()-1, tmpStr)); //初步创建Token
            }
            else{ //说明是变量名或者错误(待完善匹配错误)，没有区分标识符或者错误关键字
                tokenVector.push_back(create(TokenKind::Identifier, lineNum, keywords.size()-1, tmpStr)); 
            }
            return ;
        }
        else{
            tmpStr.push_back(tempCh);
        }
    }
}
void Lexer::scanString(){
    string tmpStr;
    while(true){
        advance();
        char tempCh = (*m_psm).at(offset_count);
        if(tempCh == '"'){ //如果扫描到双引号的结尾
            tokenVector.push_back(create(TokenKind::StringLiteral, lineNum, keywords.size()-1, tmpStr));
            return ;
        }
        else if(islastChar()){
            perror("scanString:");
            exit(-1);
        }
        else{
            tmpStr.push_back(tempCh);
        }
    }
}
void Lexer::scanNumber(){ //需要区分小数点(也可能不用区分，只需要识别小数点即可)，不能出现多个小数点
    bool isDecimal = false;
    string tmpStr;
    tmpStr.push_back((*m_psm).at(offset_count));
    while(true){
        advance();
        char tempCh = (*m_psm).at(offset_count);
        if(tempCh == '.'){
            isDecimal = true;
        }
        else if(tempCh == ' ' || tempCh == 0x0a || !isNum(tempCh)){ //如果遇到空格或者换行
            keywords.push_back(tmpStr);
            if(isDecimal){
                tokenVector.push_back(create(TokenKind::IntegerLiteral, lineNum, keywords.size()-1, tmpStr)); //添加小数Token，好像不用区分小数
            }
            else{
                tokenVector.push_back(create(TokenKind::IntegerLiteral, lineNum, keywords.size()-1, tmpStr)); //添加整数Token
            }
            return ;
        }
    }
}
bool Lexer::lookupKeyword(string targetStr, TokenKind &kind){ //查找目标子字符串是否为关键字
    //std::initializer_list<std::pair<string_view, TokenKind>> allKeywords = {KEYWORDS_1364_1995};
    unordered_map<string_view, TokenKind> allKeywords_map = {KEYWORDS_1364_1995};
    if(allKeywords_map.count(targetStr)){ //如果存在在关键字map中
        kind = allKeywords_map[targetStr];
        return true;
    }
    else{
        kind = TokenKind::StringLiteral; //说明是普通字符串
    }
    return false;
} 

vector<Token> Lexer::getTokenVector(){
    return tokenVector;
}

bool Lexer::isKeyword(TokenKind kind) {
    switch (kind) {
        case TokenKind::OneStep:
        case TokenKind::AcceptOnKeyword:
        case TokenKind::AliasKeyword:
        case TokenKind::AlwaysKeyword:
        case TokenKind::AlwaysCombKeyword:
        case TokenKind::AlwaysFFKeyword:
        case TokenKind::AlwaysLatchKeyword:
        case TokenKind::AndKeyword:
        case TokenKind::AssertKeyword:
        case TokenKind::AssignKeyword:
        case TokenKind::AssumeKeyword:
        case TokenKind::AutomaticKeyword:
        case TokenKind::BeforeKeyword:
        case TokenKind::BeginKeyword:
        case TokenKind::BindKeyword:
        case TokenKind::BinsKeyword:
        case TokenKind::BinsOfKeyword:
        case TokenKind::BitKeyword:
        case TokenKind::BreakKeyword:
        case TokenKind::BufKeyword:
        case TokenKind::BufIf0Keyword:
        case TokenKind::BufIf1Keyword:
        case TokenKind::ByteKeyword:
        case TokenKind::CaseKeyword:
        case TokenKind::CaseXKeyword:
        case TokenKind::CaseZKeyword:
        case TokenKind::CellKeyword:
        case TokenKind::CHandleKeyword:
        case TokenKind::CheckerKeyword:
        case TokenKind::ClassKeyword:
        case TokenKind::ClockingKeyword:
        case TokenKind::CmosKeyword:
        case TokenKind::ConfigKeyword:
        case TokenKind::ConstKeyword:
        case TokenKind::ConstraintKeyword:
        case TokenKind::ContextKeyword:
        case TokenKind::ContinueKeyword:
        case TokenKind::CoverKeyword:
        case TokenKind::CoverGroupKeyword:
        case TokenKind::CoverPointKeyword:
        case TokenKind::CrossKeyword:
        case TokenKind::DeassignKeyword:
        case TokenKind::DefaultKeyword:
        case TokenKind::DefParamKeyword:
        case TokenKind::DesignKeyword:
        case TokenKind::DisableKeyword:
        case TokenKind::DistKeyword:
        case TokenKind::DoKeyword:
        case TokenKind::EdgeKeyword:
        case TokenKind::ElseKeyword:
        case TokenKind::EndKeyword:
        case TokenKind::EndCaseKeyword:
        case TokenKind::EndCheckerKeyword:
        case TokenKind::EndClassKeyword:
        case TokenKind::EndClockingKeyword:
        case TokenKind::EndConfigKeyword:
        case TokenKind::EndFunctionKeyword:
        case TokenKind::EndGenerateKeyword:
        case TokenKind::EndGroupKeyword:
        case TokenKind::EndInterfaceKeyword:
        case TokenKind::EndModuleKeyword:
        case TokenKind::EndPackageKeyword:
        case TokenKind::EndPrimitiveKeyword:
        case TokenKind::EndProgramKeyword:
        case TokenKind::EndPropertyKeyword:
        case TokenKind::EndSpecifyKeyword:
        case TokenKind::EndSequenceKeyword:
        case TokenKind::EndTableKeyword:
        case TokenKind::EndTaskKeyword:
        case TokenKind::EnumKeyword:
        case TokenKind::EventKeyword:
        case TokenKind::EventuallyKeyword:
        case TokenKind::ExpectKeyword:
        case TokenKind::ExportKeyword:
        case TokenKind::ExtendsKeyword:
        case TokenKind::ExternKeyword:
        case TokenKind::FinalKeyword:
        case TokenKind::FirstMatchKeyword:
        case TokenKind::ForKeyword:
        case TokenKind::ForceKeyword:
        case TokenKind::ForeachKeyword:
        case TokenKind::ForeverKeyword:
        case TokenKind::ForkKeyword:
        case TokenKind::ForkJoinKeyword:
        case TokenKind::FunctionKeyword:
        case TokenKind::GenerateKeyword:
        case TokenKind::GenVarKeyword:
        case TokenKind::GlobalKeyword:
        case TokenKind::HighZ0Keyword:
        case TokenKind::HighZ1Keyword:
        case TokenKind::IfKeyword:
        case TokenKind::IffKeyword:
        case TokenKind::IfNoneKeyword:
        case TokenKind::IgnoreBinsKeyword:
        case TokenKind::IllegalBinsKeyword:
        case TokenKind::ImplementsKeyword:
        case TokenKind::ImpliesKeyword:
        case TokenKind::ImportKeyword:
        case TokenKind::IncDirKeyword:
        case TokenKind::IncludeKeyword:
        case TokenKind::InitialKeyword:
        case TokenKind::InOutKeyword:
        case TokenKind::InputKeyword:
        case TokenKind::InsideKeyword:
        case TokenKind::InstanceKeyword:
        case TokenKind::IntKeyword:
        case TokenKind::IntegerKeyword:
        case TokenKind::InterconnectKeyword:
        case TokenKind::InterfaceKeyword:
        case TokenKind::IntersectKeyword:
        case TokenKind::JoinKeyword:
        case TokenKind::JoinAnyKeyword:
        case TokenKind::JoinNoneKeyword:
        case TokenKind::LargeKeyword:
        case TokenKind::LetKeyword:
        case TokenKind::LibListKeyword:
        case TokenKind::LibraryKeyword:
        case TokenKind::LocalKeyword:
        case TokenKind::LocalParamKeyword:
        case TokenKind::LogicKeyword:
        case TokenKind::LongIntKeyword:
        case TokenKind::MacromoduleKeyword:
        case TokenKind::MatchesKeyword:
        case TokenKind::MediumKeyword:
        case TokenKind::ModPortKeyword:
        case TokenKind::ModuleKeyword:
        case TokenKind::NandKeyword:
        case TokenKind::NegEdgeKeyword:
        case TokenKind::NetTypeKeyword:
        case TokenKind::NewKeyword:
        case TokenKind::NextTimeKeyword:
        case TokenKind::NmosKeyword:
        case TokenKind::NorKeyword:
        case TokenKind::NoShowCancelledKeyword:
        case TokenKind::NotKeyword:
        case TokenKind::NotIf0Keyword:
        case TokenKind::NotIf1Keyword:
        case TokenKind::NullKeyword:
        case TokenKind::OrKeyword:
        case TokenKind::OutputKeyword:
        case TokenKind::PackageKeyword:
        case TokenKind::PackedKeyword:
        case TokenKind::ParameterKeyword:
        case TokenKind::PmosKeyword:
        case TokenKind::PosEdgeKeyword:
        case TokenKind::PrimitiveKeyword:
        case TokenKind::PriorityKeyword:
        case TokenKind::ProgramKeyword:
        case TokenKind::PropertyKeyword:
        case TokenKind::ProtectedKeyword:
        case TokenKind::Pull0Keyword:
        case TokenKind::Pull1Keyword:
        case TokenKind::PullDownKeyword:
        case TokenKind::PullUpKeyword:
        case TokenKind::PulseStyleOnDetectKeyword:
        case TokenKind::PulseStyleOnEventKeyword:
        case TokenKind::PureKeyword:
        case TokenKind::RandKeyword:
        case TokenKind::RandCKeyword:
        case TokenKind::RandCaseKeyword:
        case TokenKind::RandSequenceKeyword:
        case TokenKind::RcmosKeyword:
        case TokenKind::RealKeyword:
        case TokenKind::RealTimeKeyword:
        case TokenKind::RefKeyword:
        case TokenKind::RegKeyword:
        case TokenKind::RejectOnKeyword:
        case TokenKind::ReleaseKeyword:
        case TokenKind::RepeatKeyword:
        case TokenKind::RestrictKeyword:
        case TokenKind::ReturnKeyword:
        case TokenKind::RnmosKeyword:
        case TokenKind::RpmosKeyword:
        case TokenKind::RtranKeyword:
        case TokenKind::RtranIf0Keyword:
        case TokenKind::RtranIf1Keyword:
        case TokenKind::SAlwaysKeyword:
        case TokenKind::SEventuallyKeyword:
        case TokenKind::SNextTimeKeyword:
        case TokenKind::SUntilKeyword:
        case TokenKind::SUntilWithKeyword:
        case TokenKind::ScalaredKeyword:
        case TokenKind::SequenceKeyword:
        case TokenKind::ShortIntKeyword:
        case TokenKind::ShortRealKeyword:
        case TokenKind::ShowCancelledKeyword:
        case TokenKind::SignedKeyword:
        case TokenKind::SmallKeyword:
        case TokenKind::SoftKeyword:
        case TokenKind::SolveKeyword:
        case TokenKind::SpecifyKeyword:
        case TokenKind::SpecParamKeyword:
        case TokenKind::StaticKeyword:
        case TokenKind::StringKeyword:
        case TokenKind::StrongKeyword:
        case TokenKind::Strong0Keyword:
        case TokenKind::Strong1Keyword:
        case TokenKind::StructKeyword:
        case TokenKind::SuperKeyword:
        case TokenKind::Supply0Keyword:
        case TokenKind::Supply1Keyword:
        case TokenKind::SyncAcceptOnKeyword:
        case TokenKind::SyncRejectOnKeyword:
        case TokenKind::TableKeyword:
        case TokenKind::TaggedKeyword:
        case TokenKind::TaskKeyword:
        case TokenKind::ThisKeyword:
        case TokenKind::ThroughoutKeyword:
        case TokenKind::TimeKeyword:
        case TokenKind::TimePrecisionKeyword:
        case TokenKind::TimeUnitKeyword:
        case TokenKind::TranKeyword:
        case TokenKind::TranIf0Keyword:
        case TokenKind::TranIf1Keyword:
        case TokenKind::TriKeyword:
        case TokenKind::Tri0Keyword:
        case TokenKind::Tri1Keyword:
        case TokenKind::TriAndKeyword:
        case TokenKind::TriOrKeyword:
        case TokenKind::TriRegKeyword:
        case TokenKind::TypeKeyword:
        case TokenKind::TypedefKeyword:
        case TokenKind::UnionKeyword:
        case TokenKind::UniqueKeyword:
        case TokenKind::Unique0Keyword:
        case TokenKind::UnsignedKeyword:
        case TokenKind::UntilKeyword:
        case TokenKind::UntilWithKeyword:
        case TokenKind::UntypedKeyword:
        case TokenKind::UseKeyword:
        case TokenKind::UWireKeyword:
        case TokenKind::VarKeyword:
        case TokenKind::VectoredKeyword:
        case TokenKind::VirtualKeyword:
        case TokenKind::VoidKeyword:
        case TokenKind::WaitKeyword:
        case TokenKind::WaitOrderKeyword:
        case TokenKind::WAndKeyword:
        case TokenKind::WeakKeyword:
        case TokenKind::Weak0Keyword:
        case TokenKind::Weak1Keyword:
        case TokenKind::WhileKeyword:
        case TokenKind::WildcardKeyword:
        case TokenKind::WireKeyword:
        case TokenKind::WithKeyword:
        case TokenKind::WithinKeyword:
        case TokenKind::WOrKeyword:
        case TokenKind::XnorKeyword:
        case TokenKind::XorKeyword:
        case TokenKind::DefineKeyword:
            return true;
        default:
            return false;
    }
}
