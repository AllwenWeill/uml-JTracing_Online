#include "Chef.h"
class Actor {
public:
	int m_num;
	Actor();
	~Actor() {};
	void eatFood();
private:
	int getActorNum();
};