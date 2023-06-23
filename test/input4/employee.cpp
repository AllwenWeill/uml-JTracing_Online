#include "employee.h"

//构造函数
Employee::Employee(int id, string name, int dId)
{
	m_Id = id;
	m_Name = name;
	m_DeptId = dId;
}

//显示个人信息
void Employee::showInfo()
{
	cout << "worker ID: " << m_Id;
	cout << "worker name: " << m_Name;
	cout << "worker job:" << getDeptName();
	cout << "worker position: Complete the tasks assigned by the manager." << endl;
}

//获取岗位名称
string Employee::getDeptName()
{
	return  string("employee");
}