#include <string>
class Chef {
	Chef();
	~Chef();
	string getChefName() {
		return m_chefName;
	}
private:
	string m_chefName;
};