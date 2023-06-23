
#include <iostream>
using namespace std;

//经理类
class Manager
{
public:

	//构造函数
	Manager(int id, string name, int dId);

	//显示个人信息
	void showInfo();

	//获取岗位名称
	string getDeptName();

};