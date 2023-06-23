#include "Actor.h"
int sf() {
    int a = 5;
    if (a == 5) {
        getActorNum(a);
    }
    else {
        Chef CE;
        CE.cooking();
    }

    Chef* CE2 = new Chef();
    CE2->getChefName();
}

int Actor::getActorNum(int a) {
    return a;
}