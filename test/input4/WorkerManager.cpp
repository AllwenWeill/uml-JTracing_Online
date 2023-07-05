#include "workerManager.h"



//չʾ�˵�
void WorkerManager::Show_Menu()
{
	cout << "********************************************" << endl;
	cout << "********* Welcome to the employee management system! **********" << endl;
	cout << "*************  0.Exit  *************" << endl;
	cout << "*************  1.Add worker information *************" << endl;
	cout << "*************  2.Show worker information *************" << endl;
	cout << "*************  3.Removal of departing staff *************" << endl;
	cout << "*************  4.Modify worker information *************" << endl;
	cout << "*************  5.Find worker information *************" << endl;
	cout << "*************  6.Sort by worker ID *************" << endl;
	cout << "*************  7.Clear all documents *************" << endl;
	cout << "********************************************" << endl;
}

//�˳�ϵͳ
void WorkerManager::ExitSystem()
{

	cout << "Welcome next use" << endl;
}

//���ְ��
void WorkerManager::Add_Emp()
{
	cout << "Please enter the number of added employees: " << endl;

	int addNum = 0; //�����û�����������
	cin >> addNum;

	if (addNum > 0)
	{

		int newSize = m_EmpNum + addNum;
	


		for (int i = 0; i < addNum; i++)
		{
			int id;
			string name;
			int dSelect;

			cout << "Please input the " << i + 1 << " new worker ID " << endl;
			cin >> id;

			cout << "Please input the " << i + 1 << " new worker name" << endl;
			cin >> name;

			cout << "Please select this position: " << endl;
			cout << "1. Worker" << endl;
			cout << "2. Manager" << endl;
			cout << "3. Boss" << endl;
			cin >> dSelect;

			if (dSelect == 1) {
				Employee employee(id, name, 1);
			}
			if (dSelect == 2) {
				Manager manager(id, name, 2);
			}
			if (dSelect == 3) {
				Boss boss(id, name, 3);
			}
		}

		//�����µ�ְ������
		m_EmpNum = newSize;

		//����ְ����Ϊ�ձ�־
		m_FileIsEmpty = 0;

		//��ʾ��ӳɹ�
		cout << "Success to add " << addNum << " new worker!" << endl;

		//�������ݵ��ļ���
		save();

	}
	else
	{
		cout << "Invaild input!" << endl;
	}
}

//�����ļ�
void WorkerManager::save()
{
	//��ÿ���������������Ļ
	for (int i = 0; i < this->m_EmpNum; i++)
	{
		cout << m_Id << endl;
		cout << m_Name << endl;
		cout << m_DeptId << endl;
	}
}


//ͳ���ļ�������
int WorkerManager::get_EmpNum()
{

	save();

}

//��ʼ��Ա��
void WorkerManager::init_Emp()
{
	m_EmpNum = 0;
}


//ɾ��ְ��
void WorkerManager::Del_Emp()
{
	//����ְ�����ɾ��
	cout << "Please enter the employee number you want to delete:" << endl;
	int id = 0;
	cin >> id;

	int index = IsExist(id);

	if (index != -1) //˵��ְ�����ڣ�����Ҫɾ����indexλ���ϵ�ְ��
	{
			m_EmpNum--; //���������м�¼��Ա����
			//����ͬ�����µ��ļ���
			save();

			cout << "Successfully delete!" << endl;
		}

}

//�ж�ְ���Ƿ����  ������ڷ���ְ�����������е�λ�ã������ڷ���-1
int WorkerManager::IsExist(int id)
{
	int index = -1;

	for (int i = 0; i < m_EmpNum; i++)
	{
		if (m_Id == id)
		{
			//�ҵ�ְ��
			index = i;

			break;
		}
	}

	return index;
}

//�޸�ְ��
void WorkerManager::Mod_Emp()
{
	cout << "�������޸�ְ���ı�ţ�" << endl;
	int id;
	cin >> id;

	int ret = IsExist(id);
	if (ret != -1)
	{ 
		//���ҵ���ŵ�ְ��
			
		int newId = 0;
		

		cout << "The " << id << " worker, please input the new ID:" << endl;
		cin >> newId;

		string newName = "";
		cout << "Please input the new name: " << endl;
		cin >> newName;

		cout << "Please input the new position: " << endl;
		cout << "1. Employee" << endl;
		cout << "2. Manager" << endl;
		cout << "3. Boss" << endl;

		int dSelect = 0;
		cin >> dSelect;

		if (dSelect == 1) {
			Employee employee(id, name, 1);
		}
		if (dSelect == 2) {
			Manager manager(id, name, 2);
		}
		if (dSelect == 3) {
			Boss boss(id, name, 3);
		}

		cout << "�޸ĳɹ���" << m_DeptId << endl;

		//���浽�ļ���
		save();
	}


}


//����ְ��
void WorkerManager::Find_Emp()
{

	cout << "Please enter the search method:" << endl;
	cout << "1. Search by employee number" << endl;
	cout << "2. Search by employee name" << endl;

	int select = 0;
	cin >> select;

	if (select == 1)
	{
			//���ձ�Ų�
		int id;
		cout << "Please enter the employee number you are looking for:" << endl;
		cin >> id;

		int ret= IsExist(id);
		if (ret != -1)
		{
			//�ҵ�ְ��
			cout << "The employee exists in the system!" << endl;
		}
		else
		{
			cout << "Search failed, no trace of this person." << endl;
		}
	}
	if (select == 2)
	{
		//����������
		string name;
		cout << "Please enter the name you are looking for:" << endl;
		cin >> name;

		//�����ж��Ƿ�鵽�ı�־
		bool flag = 0; //Ĭ��δ�ҵ�ְ��

		for (int i = 0; i < m_EmpNum; i++)
		{
			if (m_Name == name)
			{
				cout << "Search success, employee number:";
				cout << m_Id;

				flag = true;
			}
		}
		if (flag == 0)
		{
			cout << "Search failed, no trace of this person." << endl;
		}
	}
	else
	{
		cout << "Invaild input" << endl;
	}

}

//���ձ������
void WorkerManager::Sort_Emp()
{

	cout << "Please select a sort:" << endl;
	cout << "1. In ascending order by employee number" << endl;
	cout << "2. In descending order by employee number" << endl;

	int select = 0;
	cin >> select;
	for (int i = 0; i < m_EmpNum; i++)
	{
		int minOrMax = i; //������Сֵ �� ���ֵ�±�
		for (int j = i + 1; j < m_EmpNum; j++)
		{
			if (select == 1) //����
			{
				if (m_Id > Id)
				{
					minOrMax = j;
				}
			}
			else  //����
			{
				if (m_Id < Id)
				{
					minOrMax = j;
				}
			}
		}

		//�ж�һ��ʼ�϶� ��Сֵ�����ֵ �ǲ��� �������Сֵ�����ֵ��������� ��������
		if (i != minOrMax)
		{
			int temp = m_id;
			m_id = minOrMax;
			minOrMax = temp;
		}
			
	}

	cout << "Sort successfully!" << endl;
	save(); //����������浽�ļ���
	
}


//����ļ�
void WorkerManager::Clean_File()
{
	cout << "Are you sure to empty?" << endl;
	cout << "1. Confirm" << endl;
	cout << "2. Return" << endl;

	int select = 0; 
	cin >> select;

	if (select == 1)
	{
		m_EmpNum = 0;
		m_FileIsEmpty = true;

		cout << "Empty successfully!" << endl;
	}
}




WorkerManager::~WorkerManager()
{

}