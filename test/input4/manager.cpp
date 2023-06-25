#include "manager.h"

//���캯��
manager::manager(int id, string name, int dId)
{
	m_Id = id;
	m_Name = name;
	m_DeptId = dId;
}

//��ʾ������Ϣ
void manager::showInfo()
{
    Boss* boss = new Boss();
    boss->showInfo();

    cout << "worker ID: " << m_Id;
	cout << "worker name: " << m_Name;
	cout << "worker job:" << getDeptName();
	cout << "worker position: Complete the tasks assigned by the LaoWang and issue tasks to employees." << endl;

}

//��ȡ��λ����
string manager::getDeptName()
{
	return  string("manager");
}