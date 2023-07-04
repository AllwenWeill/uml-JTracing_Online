#include "boss.h"

//构造函数
Boss::Boss(int id, string name, int dId)
{
	m_Id = id;
	m_Name = name;
	m_DeptId = dId;
}

//显示个人信息
void Boss::showInfo()
{
	cout << "worker ID: " << m_Id;
	cout << "worker name: " << m_Name;
	cout << "worker job:" << getDeptName();
	cout << "worker position: Manage all business of company." << endl;
}

//获取岗位名称
string Boss::getDeptName()
{
	return  string("Boss");
}