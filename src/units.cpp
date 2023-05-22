#include "units.hpp"
#include <time.h>

void unit::move(double x ,double y){
    
}
void unit::attack(unit* b){
    b->hp -= 10;
    cout << "one unit attacked ! hp = " << b->hp << endl;
    t_a = ATTACK_COOLDOWN;
}
void unit::wait(time_t t){
    
}

