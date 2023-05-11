#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>


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
    void readSource(fs::path& filepath);
    void readSource(const string& codeText);
    ~SourceManager() {};
    bool scanFileDir();
private:
    fs::path m_filepath; //文件路径
    fs::path m_fileInputDirPath; //输入文件夹路径
    fs::path m_fileOutputDirPath; //输出文件夹路径
    vector<string> m_filenames; //扫描到的input文件夹里的文件名
    vector<string> m_classnames; //扫描.h文件得到的类名
    void filterClassName(string filename);
};
