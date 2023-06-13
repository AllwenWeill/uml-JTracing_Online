#include "SourceManager.h"
namespace fs = std::filesystem;
SourceManager::SourceManager(fs::path& filepath) :
        m_filepath(filepath),
        m_fileInputDirPath("/tmp/tmp.V41aZ2znkH/test/input")
{
    scanFileDir();
}

SourceManager::SourceManager(const string& codeText)
{
    readSource(codeText);
}
// function: 获取input文件夹中所有文件的文件名
// param: null
// return: null
bool SourceManager::scanFileDir() {
    //判断.sv文件路径是否存在
    if (!fs::exists(m_fileInputDirPath)) {
        perror("Error: Invaild fileInputDirPath.\n");
        return false;
    }
    // 遍历 input 文件夹中的每一个文件
    for (const auto& dirEntry : std::filesystem::directory_iterator(m_fileInputDirPath)) {
        if (dirEntry.is_regular_file()) {
            const std::string& fileName = dirEntry.path().filename().string();
            m_filenames.emplace_back(fileName);
        }
    }
    // debug: 打印文件名
    cout << "<File names> " << endl;
    for (const auto& fileName : m_filenames) {
        std::cout << fileName << std::endl;
        filterFileName(fileName);
    }
    cout << "-------------------------" << endl;
    for (const auto& [key, value] : m_hfiles) {
        std::cout << key << " : " << std::endl << value << std::endl;
    }
    for (const auto& [key, value] : m_cppfiles) {
        std::cout << key << " : " << std::endl << value << std::endl;
    }
    cout << "-------------------------" << endl;
    if (m_filenames.empty())
        return false;
    return true;
}

void SourceManager::filterFileName(string filename) {
    //检测后缀三个字符是否为".h"
    if (filename.substr(filename.size() - 2, filename.size() - 1) == ".h") { //添加.h文本内容
        string str_filepath = "/tmp/tmp.V41aZ2znkH/test/input/" + filename;
        fs::path filepath(str_filepath);
        m_hfiles[filename] = readNormalSource(filepath); //读取文本
        /*
        //检测首字母是否为大写
        if (isupper(filename[0])) {
            string tmpClassName = filename.substr(0, filename.size() - 2);
            m_classnames.emplace_back(tmpClassName);
        }*/
    }
    else if (filename.substr(filename.size() - 4, filename.size() - 1) == ".cpp") { //添加.cpp文本内容
        string str_filepath = "/tmp/tmp.V41aZ2znkH/test/input/" + filename;
        fs::path filepath(str_filepath);
        m_cppfiles[filename] = readNormalSource(filepath); //读取文本
    }
    else {
        return;
    }
}

// function: 读取.h或.cpp文件文本内容
// param: 文件路径
// return: 文件文本
string SourceManager::readNormalSource(fs::path& filepath) {
    //验证文件路径有效性
    if (!fs::exists(filepath)) {
        perror("Error: Invaild filepath.\n");
        exit(-1); //修改为重新输入路径
    }
    std::ifstream stream(filepath, std::ios::binary);
    std::error_code ec;
    int filesize = fs::file_size(filepath, ec);
    vector<char> buffer;
    buffer.resize((size_t)filesize + 1);
    if (!stream.read(buffer.data(), (std::streamsize)filesize)) {
        perror("stream read failed!\n");
        exit(-1);
    }
    string filememo = string(buffer.begin(), buffer.end());
    return filememo;
}

unordered_map<string, string> SourceManager::gethFiles() {
    return m_hfiles;
}

unordered_map<string, string> SourceManager::getcppFiles() {
    return m_cppfiles;
}

// function: 读取源代码.sv文件
// param: .sv文件的路径
// return: null
void SourceManager::readsvSource(fs::path& filepath) {
    //判断.sv文件路径是否存在
    if (!fs::exists(filepath)) {
        perror("Error: Invaild filepath.\n");
        exit(-1); //修改为重新输入路径
    }
    fd.filename = filepath.filename().string();
    //检测后缀三个字符是否为".sv"
    if (fd.filename.substr(fd.filename.size() - 3, fd.filename.size() - 1) != ".sv") {
        perror("Error: Not the .sv file.\n");
        exit(-1);//修改为重新输入路径
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