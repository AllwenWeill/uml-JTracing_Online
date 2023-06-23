#include "manager.h"

//构造函数
Manager::Manager(int id, string name, int dId)
{
	m_Id = id;
	m_Name = name;
	m_DeptId = dId;
}

//显示个人信息
void Manager::showInfo()
{
	cout << "worker ID: " << m_Id;
	cout << "worker name: " << m_Name;
	cout << "worker job:" << getDeptName();
	cout << "worker position: Complete the tasks assigned by the LaoWang and issue tasks to employees." << endl;
}

//获取岗位名称
string Manager::getDeptName()
{
	return  string("manager");
}