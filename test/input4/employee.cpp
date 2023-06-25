#include "employee.h"

//���캯��
Employee::Employee(int id, string name, int dId)
{
	m_Id = id;
	m_Name = name;
	m_DeptId = dId;
}

//��ʾ������Ϣ
void Employee::showInfo()
{
    getDeptName();
	cout << "worker ID: " << m_Id;
	cout << "worker name: " << m_Name;
	cout << "worker job:" << getDeptName();
	cout << "worker position: Complete the tasks assigned by the manager." << endl;
}

//��ȡ��λ����
string Employee::getDeptName()
{
	return  string("employee");
}