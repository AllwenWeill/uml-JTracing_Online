#include <iostream>
#include "workerManager.h"
using  namespace std;


#include "employee.h"
#include "manager.h"
#include "boss.h"

int main()
{

	Employee* employee = new Employee(1, "ZhangSan", 1);
	employee->showInfo();

	Manager* manager = new Manager(2, "LiSi", 2);
	manager->showInfo();

	Boss* boss = new Boss(3, "Bruce", 0);
	boss->showInfo();


	WorkerManager wm;


	while (1)
	{

		wm.Show_Menu();
		int choice = 0; 
		cout << "Please enter your selection: " << endl;
		cin >> choice; 

		if (choice == 0) {	
			wm.ExitSystem();
		}
		if (choice == 1) {	
			wm.Add_Emp();
		}
		if (choice == 3) {	
			wm.Del_Emp();
		}
		if (choice == 4) {	
			wm.Mod_Emp();
		}
		if (choice == 5) {	
			wm.Find_Emp();
		}
		if (choice == 6) {	
			wm.Sort_Emp();
		}
		if (choice == 7) {	
			wm.Clean_File();
		}



	}


	return 0;
}