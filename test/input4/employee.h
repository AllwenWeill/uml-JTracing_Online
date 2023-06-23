//普通员工文件
#include <iostream>
using namespace std;

class Employee
{
public:

	//构造函数
	Employee(int id , string name , int dId);

	//显示个人信息
	void showInfo();

	//获取岗位名称
	string getDeptName();

};