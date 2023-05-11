#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>


using namespace std;
namespace fs = std::filesystem;
struct FileData {
    string filename; //�ļ���
    string filememo; //�ļ�����
    string filedirectory; //�ļ�Ŀ¼
    unsigned long int filesize; //�ļ���С
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
    fs::path m_filepath; //�ļ�·��
    fs::path m_fileInputDirPath; //�����ļ���·��
    fs::path m_fileOutputDirPath; //����ļ���·��
    vector<string> m_filenames; //ɨ�赽��input�ļ�������ļ���
    vector<string> m_classnames; //ɨ��.h�ļ��õ�������
    void filterClassName(string filename);
};
