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
//function: ɨ��׶Σ� ��Ҫ�������һЩ����Ҫ���ɴʷ���Ԫ�ļ򵥴�������ɾ��ע�ͺͽ���������Ŀհ��ַ�ѹ����һ���ַ���Ȼ�����������
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
            case '\n': //��ʾ����
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
                case '*':   //��ע��
                    scanBlockComment();
                    break;
                case '/':   //��ע��
                    scanLineComment();
                    break;
                default:  //˵������"/"����Ҫ����error��Ϣ
                    break;
                }
                break;
            default:  //˵��Ϊ��ͨ�����ַ������ǿո�ע�͵�trivial things
                if(isChar(tempCh)){ //�������ĸ���ؼ��֣��������ƣ�
                    scanLetter(); //����ɨ�赱ǰ��ĸ�����˴������ƣ�����������_�»��߿�ͷ��
                    break;
                }
                else if(tempCh == '"'){ //�����˫���ţ���Ϊ�ַ����ı�
                    scanString(); //����ɨ���ַ���
                    advance();
                }
                else if(isNum(tempCh)){ //���������
                    scanNumber(); //����ɨ�赱ǰ���ִ���ע������С����
                    break;
                }
                else if(isOperator(tempCh)){ //����������
                    switch(tempCh)
                    {
                        keywords.push_back(tempStr);
                        tempStr.clear();
                        tempStr.push_back(tempCh);
                        keywords.push_back(tempStr);
                        tempStr.clear(); //�������к����࣬���Ƚ�charתstring��ѹ������
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
                else if(isCmpOperator(tempCh)){ //����ǱȽϷ�
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
                        else if(nextCh == ' '){ //˵��ֻ��һ���Ⱥ�=
                            keywords.push_back(tempStr);
                            tokenVector.push_back(create(TokenKind::Equals, lineNum, keywords.size()-1, "="));
                            tempStr.clear();
                            break;
                        }
                        else{ //������

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
                        else{ //�����Ǹ�ֵ��䣬�ɻ��е�����>��<�Ƚ����
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
                        else{ //�����Ǹ�ֵ��䣬�ɻ��е�����>��<�Ƚ����
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
                else if(tempCh == '('){ //����������ı����޿ո����������޷���⵽
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
    /*cout << "test:��ӡkeywords����Ԫ��" << endl;
    for(string str : keywords){
        // for(auto ch : str)
        //     printf("%x ", ch);
        cout<<str<<" "<<endl;
    }*/
    cout<<"TokenKind:------------"<<endl;
    for(auto k : tokenVector){
        cout<<k.getTokenStr()<<":"<<k.getTokenKindStr()<<"�к�:"<<k.TL.m_tokenLine<<endl;
    }
}
void Lexer::advance(){
    offset_count++;
}
void Lexer::advance(int count){
    offset_count += count;
}
bool Lexer::islastChar(){//��'/0'Ҳ�����жϣ����ǲ���
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
        if (tempCh == '*' && (*m_psm).at(offset_count+1) == '/') { //�˴�offset_count+1��Խ�����
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
        if (tempCh == 0x0a) {//0x0a��ʾ����
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
        //if(tempCh == '(') //������Ҫ��ɨ��D(x)�������
        if(tempCh == ' ' || tempCh == 0x0a || !isChar(tempCh)){ //��������ո���߻���
            keywords.push_back(tmpStr);
            TokenKind kind;
            if(lookupKeyword(tmpStr, kind)){ //˵���ǹؼ���
                if (kind == TokenKind::ClassKeyword) {
                    isClassName = true;
                }
                tokenVector.push_back(create(kind, lineNum, keywords.size()-1, tmpStr)); //��������Token
            }
            else{ //˵���Ǳ��������ߴ���(������ƥ�����)��û�����ֱ�ʶ�����ߴ���ؼ���
                tokenVector.push_back(create(TokenKind::Identifier, lineNum, keywords.size()-1, tmpStr)); 
                if (isClassName) { //����ñ�����ʶ��ǰ����class�ؼ��֣���˵�������������
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
        if(tempCh == '"'){ //���ɨ�赽˫���ŵĽ�β
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
void Lexer::scanNumber(){ //��Ҫ����С����(Ҳ���ܲ������֣�ֻ��Ҫʶ��С���㼴��)�����ܳ��ֶ��С����
    bool isDecimal = false;
    string tmpStr;
    tmpStr.push_back((*m_psm).at(offset_count));
    while(true){
        advance();
        char tempCh = (*m_psm).at(offset_count);
        if(tempCh == '.'){
            isDecimal = true;
        }
        else if(tempCh == ' ' || tempCh == 0x0a || !isNum(tempCh)){ //��������ո���߻���
            keywords.push_back(tmpStr);
            if(isDecimal){
                tokenVector.push_back(create(TokenKind::IntegerLiteral, lineNum, keywords.size()-1, tmpStr)); //���С��Token������������С��
            }
            else{
                tokenVector.push_back(create(TokenKind::IntegerLiteral, lineNum, keywords.size()-1, tmpStr)); //�������Token
            }
            return ;
        }
    }
}
bool Lexer::lookupKeyword(string targetStr, TokenKind &kind){ //����Ŀ�����ַ����Ƿ�Ϊ�ؼ���
    //std::initializer_list<std::pair<string_view, TokenKind>> allKeywords = {KEYWORDS_1364_1995};
    unordered_map<string_view, TokenKind> allKeywords_map = {KEYWORDS_1364_1995};
    if(allKeywords_map.count(targetStr)){ //��������ڹؼ���map��
        kind = allKeywords_map[targetStr];
        return true;
    }
    else{
        kind = TokenKind::StringLiteral; //˵������ͨ�ַ���
    }
    return false;
} 

string Lexer::getClassName() {
    return m_className;
}

vector<Token> Lexer::getTokenVector(){
    return tokenVector;
}


