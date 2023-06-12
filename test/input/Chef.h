#include <string>
#include <iostream>
class Chef {
public:
	Chef();
	~Chef();
	string getChefName();
	void cooking();
private:
	string m_chefName;
};