#pragma once

#include <vector>
#include "player.hpp"
using namespace std;

enum class uType {};

class unit {
public:
    unit(double x, double y){
        this->x = x;
        this->y = y;
    }
//actions
    void move(double x ,double y);
    void attack(unit* b);
    void wait(time_t t);

private:
    double x,y;
    int hp;
    int t_a, t_m;
    uType type;
};

class unitAction{
private:
    unit u;
    uType type;
    unit cible;
    time_t t;
};