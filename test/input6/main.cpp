#include "Actor.h"
#include "Chef.h"

int main() {
    int a = 5;
    if (a == 5) {
        Actor A;
        A.getActorNum(a);
    }
    else {
        Chef CE;
        CE.cooking();
    }

    Chef* CE2 = new Chef();
    CE2->getChefName();
}