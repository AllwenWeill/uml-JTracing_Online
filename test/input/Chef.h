#include <string>
#include <iostream>
class Chef {
public:
	Chef();
	~Chef();
	string getChefName() {
		return m_chefName;
	}
	void cooking() {
		std::cout << "The chef is cooking..." << std::endl;
	}
private:
	string m_chefName;
};