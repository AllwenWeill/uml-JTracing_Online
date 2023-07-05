#include "Actor.h"
#include "Chef.h"

double main() {
    double  a = 5;
    if (a == 5) {
        Actor A;
        A.getActorNum(a);
    }
    if(a ==3) {
        Chef CE;
        CE.cooking();
    }

    Chef* CE2 = new Chef();
    CE2->getChefName();
    return 0;
}