#include "boss.h"

//���캯��
Boss::Boss(int id, string name, int dId)
{
	m_Id = id;
	m_Name = name;
	m_DeptId = dId;
}

//��ʾ������Ϣ
void Boss::showInfo()
{
	cout << "worker ID: " << m_Id;
	cout << "worker name: " << m_Name;
	cout << "worker job:" << getDeptName();
	cout << "worker position: Manage all business of company." << endl;
}

//��ȡ��λ����
string Boss::getDeptName()
{
	return  string("Boss");
}