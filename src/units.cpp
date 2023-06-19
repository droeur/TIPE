#include "units.hpp"
#include <time.h>

void unit::move(double x ,double y){
    
}
void unit::attack(object_abstract_class* b){
    b->removeHP(1);
    if(b->getHP() < 0){
        b->setHP(0);
    }
    cout << "unit " << this << " ,player " << (int)_joueur << " attacked unit " << b << " ,player " << (int)b->getPlayer() << " hp = " << b->getHP() << endl;
    t_a = ATTACK_COOLDOWN;
}
void unit::wait(time_t t){
    
}

