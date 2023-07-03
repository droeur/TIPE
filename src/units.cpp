#include "units.hpp"
#include "food.hpp"

using namespace std;

void unit_class::move(double x ,double y){
    
}
void unit_class::attack(object_abstract_class* b, state_class* s){
    b->hp_remove(1);
    if(b->hp_get() < 0){
        b->hp_set(0);
        if (b->object_type_get() == object_type::unit)
        {
            if (auto* u = dynamic_cast<unit_class*>(b); u->carry_food_get())
            {
                const auto f = new food_class{u->q_get(), u->r_get()};
                s->food_append(f);
            }
        }
    }
    std::cout << "unit " << this << " ,player " << player_ << " attacked unit " << b << " ,player "
              << b->player_get() << " hp = " << b->hp_get() << std::endl;
    t_a_ = ATTACK_COOLDOWN;
}
void unit_class::wait(time_t t){
    
}

