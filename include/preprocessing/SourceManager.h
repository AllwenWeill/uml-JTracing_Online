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
    void readsvSource(fs::path& filepath);
    string readNormalSource(fs::path& filepath); //��ȡ.h/.cpp�ļ�
    void readSource(const string& codeText);
    ~SourceManager() {};
    bool scanFileDir();
    unordered_map<string, string> gethFiles();
    unordered_map<string, string> getcppFiles();
private:
    fs::path m_filepath; //�ļ�·��
    fs::path m_fileInputDirPath; //�����ļ���·��
    fs::path m_fileOutputDirPath; //����ļ���·��
    vector<string> m_filenames; //ɨ�赽��input�ļ�������ļ���
    vector<string> m_classnames; //ɨ��.h�ļ��õ�������
    unordered_map<string, string> m_hfiles; //����.hͷ�ļ������ı�����
    unordered_map<string, string> m_cppfiles; //����.cppͷ�ļ������ı�����

    void filterFileName(string filename);
};
#endif