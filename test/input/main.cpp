#include "Actor.h"
#include "Chef.h"
#include "test.h"
int main() {
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
main::getActorNum(int a ){
    return a;
}