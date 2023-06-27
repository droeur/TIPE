#include "units.hpp"

using namespace std;

void unit::move(double x ,double y){
    
}
void unit::attack(object_abstract_class* b){
    b->hp_remove(1);
    if(b->hp_get() < 0){
        b->hp_set(0);
    }
    std::cout << "unit " << this << " ,player " << (int)player_ << " attacked unit " << b << " ,player "
              << (int)b->player_get() << " hp = " << b->hp_get() << std::endl;
    t_a = ATTACK_COOLDOWN;
}
void unit::wait(time_t t){
    
}

