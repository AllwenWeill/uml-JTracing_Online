#include "Lexer.h"
Lexer::Lexer(const string *psm, unsigned long int indexOffset)
    :m_psm(psm),
    init_psm(psm),
    m_indexOffset(indexOffset)
{
    m_className = "";
    offset_count = 0;
    lineNum = 1;
    keywoedsCount = 0;
    isClassName = false;
    scanText();
}
//function: 扫描阶段： 主要负责完成一些不需要生成词法单元的简单处理，比如删除注释和将多个连续的空白字符压缩成一个字符，然后产生词素流
void Lexer::scanText(){
    string tempStr;
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
    /*cout << "test:打印keywords所有元素" << endl;
    for(string str : keywords){
        // for(auto ch : str)
        //     printf("%x ", ch);
        cout<<str<<" "<<endl;
    }*/
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
                if (kind == TokenKind::ClassKeyword) {
                    isClassName = true;
                }
                tokenVector.push_back(create(kind, lineNum, keywords.size()-1, tmpStr)); //初步创建Token
            }
            else{ //说明是变量名或者错误(待完善匹配错误)，没有区分标识符或者错误关键字
                tokenVector.push_back(create(TokenKind::Identifier, lineNum, keywords.size()-1, tmpStr)); 
                if (isClassName) { //如果该变量标识符前面是class关键字，则说明后面的是类名
                    m_className = tmpStr;
                    isClassName = false;
                }
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

string Lexer::getClassName() {
    return m_className;
}

vector<Token> Lexer::getTokenVector(){
    return tokenVector;
}


