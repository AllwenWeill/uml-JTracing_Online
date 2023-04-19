#include "SourceManager.h"
namespace fs = std::filesystem;
SourceManager::SourceManager(fs::path& filepath) :
    m_filepath(filepath)
{
    readSource(m_filepath);
    cout << "构造中显示：" << endl;
    cout << fd.filememo << endl;
}

SourceManager::SourceManager(const string& codeText)
{
    readSource(codeText);
}

// function: 读取源代码.sv文件
// param: .sv文件的路径
// return: null
void SourceManager::readSource(fs::path& filepath) {
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
    cout << fd.filememo << endl;
}

void SourceManager::readSource(const string& codeText) {
    fd.filesize = codeText.size();
    fd.filename = "null";
    fd.filedirectory = "null";
    fd.filememo = codeText;
}