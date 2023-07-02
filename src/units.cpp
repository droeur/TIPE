#include "units.hpp"
#include "food.hpp"

using namespace std;

void unit::move(double x ,double y){
    
}
void unit::attack(object_abstract_class* b, state_class &s){
    b->hp_remove(1);
    if(b->hp_get() < 0){
        b->hp_set(0);
        if (b->object_type_get() == object_type::unit)
        {
            if (auto* u = dynamic_cast<unit*>(b); u->carry_food_get())
            {
                const food_class f{u->q_get(), u->r_get()};
                s.food_append(f);
            }
        }
    }
    std::cout << "unit " << this << " ,player " << (int)player_ << " attacked unit " << b << " ,player "
              << (int)b->player_get() << " hp = " << b->hp_get() << std::endl;
    t_a = ATTACK_COOLDOWN;
}
void unit::wait(time_t t){
    
}

