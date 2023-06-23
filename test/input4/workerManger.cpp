#include "workerManager.h"

WorkerManager::WorkerManager()
{

	//1、文件不存在

	ifstream ifs;
	ifs.open(FILENAME); // 读文件

	if (!ifs.is_open())
	{
		cout << "Cannot find file!" << endl;
		//初始化属性
		//初始化记录人数
		m_EmpNum = 0;
		//初始化文件是否为空
		m_FileIsEmpty = 1;
		ifs.close();
		return;
	}

	//2、文件存在 数据为空
	char ch;
	ifs >> ch;
	if (ifs.eof())
	{
		//文件为空
		cout << "File empty!" << endl;
		//初始化记录人数
		m_EmpNum = 0;
		//初始化文件是否为空
		m_FileIsEmpty = 1;
		ifs.close();
		return;
	}

	//3、文件存在，并且记录数据
	int num = get_EmpNum();
	cout << "Staff number: " << num << endl;
	m_EmpNum = num;
	
	//将文件中的数据 ，存到数组中
	init_Emp();

}

//展示菜单
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

//退出系统
void WorkerManager::ExitSystem()
{
	cout << "Welcome next use" << endl;
}

//添加职工
void WorkerManager::Add_Emp()
{
	cout << "Please enter the number of added employees: " << endl;

	int addNum = 0; //保存用户的输入数量
	cin >> addNum;

	if (addNum > 0)
	{
		//添加
		//计算添加新空间大小
		int newSize = m_EmpNum + addNum; // 新空间人数 = 原来记录人数 + 新增人数
	

		//批量添加新数据
		for (int i = 0; i < addNum; i++)
		{
			int id; //职工编号
			string name; //职工姓名
			int dSelect; // 部门选择

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

		//更新新的职工人数
		m_EmpNum = newSize;

		//更新职工不为空标志
		m_FileIsEmpty = 0;

		//提示添加成功
		cout << "Success to add " << addNum << " new worker!" << endl;

		//保存数据到文件中
		save();

	}
	else
	{
		cout << "Invaild input!" << endl;
	}
}

//保存文件
void WorkerManager::save()
{
	//将每个人数据输出到屏幕
	for (int i = 0; i < this->m_EmpNum; i++)
	{
		cout << m_Id << endl;
		cout << m_Name << endl;
		cout << m_DeptId << endl;
	}
}


//统计文件中人数
int WorkerManager::get_EmpNum()
{

	return m_EmpNum;

}

//初始化员工
void WorkerManager::init_Emp()
{
	m_EmpNum = 0;
}


//删除职工
void WorkerManager::Del_Emp()
{
	//按照职工编号删除
	cout << "Please enter the employee number you want to delete:" << endl;
	int id = 0;
	cin >> id;

	int index = IsExist(id);

	if (index != -1) //说明职工存在，并且要删除掉index位置上的职工
	{
			m_EmpNum--; //更新数组中记录人员个数
			//数据同步更新到文件中
			save();

			cout << "Successfully delete!" << endl;
		}

}

//判断职工是否存在  如果存在返回职工所在数组中的位置，不存在返回-1
int WorkerManager::IsExist(int id)
{
	int index = -1;

	for (int i = 0; i < m_EmpNum; i++)
	{
		if (m_Id == id)
		{
			//找到职工
			index = i;

			break;
		}
	}

	return index;
}

//修改职工
void WorkerManager::Mod_Emp()
{
	cout << "请输入修改职工的编号：" << endl;
	int id;
	cin >> id;

	int ret = IsExist(id);
	if (ret != -1)
	{ 
		//查找到编号的职工
			
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

		cout << "修改成功！" << m_DeptId << endl;

		//保存到文件中
		save();
	}


}


//查找职工
void WorkerManager::Find_Emp()
{

	cout << "Please enter the search method:" << endl;
	cout << "1. Search by employee number" << endl;
	cout << "2. Search by employee name" << endl;

	int select = 0;
	cin >> select;

	if (select == 1)
	{
			//按照编号查
		int id;
		cout << "Please enter the employee number you are looking for:" << endl;
		cin >> id;

		int ret= IsExist(id);
		if (ret != -1)
		{
			//找到职工
			cout << "The employee exists in the system!" << endl;
		}
		else
		{
			cout << "Search failed, no trace of this person." << endl;
		}
	}
	if (select == 2)
	{
		//按照姓名查
		string name;
		cout << "Please enter the name you are looking for:" << endl;
		cin >> name;

		//加入判断是否查到的标志
		bool flag = 0; //默认未找到职工

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

//按照编号排序
void WorkerManager::Sort_Emp()
{

	cout << "Please select a sort:" << endl;
	cout << "1. In ascending order by employee number" << endl;
	cout << "2. In descending order by employee number" << endl;

	int select = 0;
	cin >> select;
	for (int i = 0; i < m_EmpNum; i++)
	{
		int minOrMax = i; //声明最小值 或 最大值下标
		for (int j = i + 1; j < m_EmpNum; j++)
		{
			if (select == 1) //升序
			{
				if (m_Id > Id)
				{
					minOrMax = j;
				}
			}
			else  //降序
			{
				if (m_Id < Id)
				{
					minOrMax = j;
				}
			}
		}

		//判断一开始认定 最小值或最大值 是不是 计算的最小值或最大值，如果不是 交换数据
		if (i != minOrMax)
		{
			int temp = m_id;
			m_id = minOrMax;
			minOrMax = temp;
		}
			
	}

	cout << "Sort successfully!" << endl;
	save(); //排序后结果保存到文件中
	
}


//清空文件
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