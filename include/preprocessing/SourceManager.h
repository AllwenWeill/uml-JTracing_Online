#ifndef _SOURCEMANAGER_H
#define _SOURCEMANAGER_H
#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <unordered_map>


using namespace std;
namespace fs = std::filesystem;
struct FileData {
    string filename; //文件名
    string filememo; //文件内容
    string filedirectory; //文件目录
    unsigned long int filesize; //文件大小
};
class SourceManager {
public:
    FileData fd;
    SourceManager(fs::path& filepath);
    SourceManager(const string& codeText);
    void readsvSource(fs::path& filepath);
    string readNormalSource(fs::path& filepath); //读取.h/.cpp文件
    void readSource(const string& codeText);
    ~SourceManager() {};
    bool scanFileDir();
    unordered_map<string, string> gethFiles();
    unordered_map<string, string> getcppFiles();
private:
    fs::path m_filepath; //文件路径
    fs::path m_fileInputDirPath; //输入文件夹路径
    fs::path m_fileOutputDirPath; //输出文件夹路径
    vector<string> m_filenames; //扫描到的input文件夹里的文件名
    vector<string> m_classnames; //扫描.h文件得到的类名
    unordered_map<string, string> m_hfiles; //仅含.h头文件及其文本内容
    unordered_map<string, string> m_cppfiles; //仅含.cpp头文件及其文本内容

    void filterFileName(string filename);
};
#endif