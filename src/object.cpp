#include "object.hpp"


void object_abstract_class::temporary_hp_set(const int hp)
{
    temporary_hp_ = hp;
    if (temporary_hp_ < 0)
        temporary_hp_ = 0;
}

void object_abstract_class::temporary_hp_remove(const int hp)
{
    temporary_hp_ = hit_point_ - hp;
    if (temporary_hp_ < 0)
        temporary_hp_ = 0;
}

void object_abstract_class::temporary_hp_apply()
{
    hit_point_ = temporary_hp_;
}

int object_abstract_class::temporary_hp_get()
{
    return temporary_hp_;
}