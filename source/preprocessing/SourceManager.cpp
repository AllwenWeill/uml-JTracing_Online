#include "SourceManager.h"
namespace fs = std::filesystem;
SourceManager::SourceManager(fs::path& filepath) :
    m_filepath(filepath),
    m_fileInputDirPath("../../../test/input")
{
    readSource(m_filepath);
    scanFileDir();
}

SourceManager::SourceManager(const string& codeText)
{
    readSource(codeText);
}
// function: ��ȡinput�ļ����������ļ����ļ���
// param: null
// return: null
bool SourceManager::scanFileDir() {
    //�ж�.sv�ļ�·���Ƿ����
    if (!fs::exists(m_fileInputDirPath)) {
        perror("Error: Invaild fileInputDirPath.\n");
        return false; 
    }
    // ���� input �ļ����е�ÿһ���ļ�
    for (const auto& dirEntry : std::filesystem::directory_iterator(m_fileInputDirPath)) {
        if (dirEntry.is_regular_file()) {
            const std::string& fileName = dirEntry.path().filename().string();
            m_filenames.emplace_back(fileName);
        }
    }
    // ��ӡ�ļ���
    for (const auto& fileName : m_filenames) {
        std::cout << fileName << std::endl;
        filterClassName(fileName);
    }
    cout << "class:";
    for (const auto& className : m_classnames) {
        cout << className << endl;
    }
    if (m_filenames.empty())
        return false;
    return true;
}

void SourceManager::filterClassName(string filename) {
    //����׺�����ַ��Ƿ�Ϊ".h"
    if (filename.substr(filename.size() - 2, filename.size() - 1) != ".h") {
        return ;//
    }
    //�������ĸ�Ƿ�Ϊ��д
    if (isupper(filename[0])) {
        string tmpClassName = filename.substr(0, filename.size() - 2);
        m_classnames.emplace_back(tmpClassName);
    }
}

// function: ��ȡԴ����.sv�ļ�
// param: .sv�ļ���·��
// return: null
void SourceManager::readSource(fs::path& filepath) {
    //�ж�.sv�ļ�·���Ƿ����
    if (!fs::exists(filepath)) {
        perror("Error: Invaild filepath.\n");
        exit(-1); //�޸�Ϊ��������·��
    }
    fd.filename = filepath.filename().string();
    //����׺�����ַ��Ƿ�Ϊ".sv"
    if (fd.filename.substr(fd.filename.size() - 3, fd.filename.size() - 1) != ".sv") {
        perror("Error: Not the .sv file.\n");
        exit(-1);//�޸�Ϊ��������·��
    }
    fd.filedirectory = filepath.string();
    std::ifstream stream(filepath, std::ios::binary);
    std::error_code ec;
    fd.filesize = fs::file_size(filepath, ec);
    vector<char> buffer;
    buffer.resize((size_t)fd.filesize + 1);
    if (!stream.read(buffer.data(), (std::streamsize)fd.filesize)) {
        perror("stream read failed!\n");
        exit(-1);
    }
    fd.filememo = string(buffer.begin(), buffer.end());
}

void SourceManager::readSource(const string& codeText) {
    fd.filesize = codeText.size();
    fd.filename = "null";
    fd.filedirectory = "null";
    fd.filememo = codeText;
}