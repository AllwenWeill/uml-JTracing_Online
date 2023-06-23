#include <iostream>
using namespace std;

//老板类
class Boss
{
public:

	//构造函数
	Boss(int id, string name, int dId);

	//显示个人信息
	void showInfo();

	//获取岗位名称
	string getDeptName();

};