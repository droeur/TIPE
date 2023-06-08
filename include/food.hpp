#pragma once

#include "position.hpp"

class food_class{
public:
    food_class(int q, int r){
        p.setQ(q);
        p.setR(r);
    }
private:
    position p;
};