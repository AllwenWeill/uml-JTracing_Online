#include <string>
#include <iostream>
class Chef {
public:
	Chef();
	~Chef();
	string getChefName() {
		return m_chefName;
	}
	void cooking();
private:
	string m_chefName;
};