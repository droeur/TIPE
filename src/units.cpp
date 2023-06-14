#include "units.hpp"
#include <time.h>

void unit::move(double x ,double y){
    
}
void unit::attack(unit* b){
    b->hp -= 10;
    if(b->hp < 0){
        b->hp = 0;
    }
    cout << "unit " << this << " ,player " << _joueur << " attacked unit " << b << " ,player " << b->_joueur << " hp = " << b->hp << endl;
    t_a = ATTACK_COOLDOWN;
}
void unit::wait(time_t t){
    
}

