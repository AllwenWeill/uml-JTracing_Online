#include "Chef.h"
#include "Test.h"

void Chef:: cooking() {
    Test c2;
    c2.buyFood();
    std::cout << "The chef is cooking..." << std::endl;

}
string Chef:: getChefName() {
    return "zzx";
}