#include "Actor.h"
Actor::Actor() {

}
int Actor::getActorNum(){
	return m_num;
}

void Actor::eatFood() {
	Chef CE;
	CE.cooking();
	Chef* CE2 = new Chef();
	CE2->getChefName();
}