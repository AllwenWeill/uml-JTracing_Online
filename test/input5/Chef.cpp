#include "Chef.h"
#include "Test.h"

void Chef::cooking() {
    Test c2;
    int a =0;
    c2.buyFood(a);
    std::cout << "The chef is cooking..." << std::endl;

}
string Chef::getChefName() {
    return "zzx";
}