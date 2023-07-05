#include <iostream>
#include "workerManager.h"
using  namespace std;


#include "employee.h"
#include "manager.h"
#include "boss.h"

int main()
{
    WorkerManager wm;
    wm.Add_Emp();
	Employee * employee = new Employee(1, "ZhangSan", 1);
	employee->showInfo();

	Manager* manager = new Manager();
    manager->showInfo();

	Boss* boss = new Boss(3, "Bruce", 0);
    boss->showInfo();

	//ʵ���������߶���


	while (1)
	{
		//����չʾ�˵���Ա����
		wm.Show_Menu();
		int choice = 0; //�����洢�û���ѡ��
		cout << "Please enter your selection: " << endl;
		cin >> choice; // �����û���ѡ��

		if (choice == 0) {	//�˳�ϵͳ
			wm.ExitSystem();
		}
		if (choice == 1) {	//����ְ��
			wm.Add_Emp();
		}
		if (choice == 3) {	//ɾ��ְ��
			wm.Del_Emp();
		}
		if (choice == 4) {	//�޸�ְ��
			wm.Mod_Emp();
		}
		if (choice == 5) {	//����ְ��
			wm.Find_Emp();
		}
		if (choice == 6) {	//����ְ��
			wm.Sort_Emp();
		}
		if (choice == 7) {	//����ĵ�
			wm.Clean_File();
		}



	}


	return 0;
}