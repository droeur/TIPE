#pragma once

#include <vector>
#include "game.hpp"

using namespace std;

typedef int frame;

enum class uType {NORMAL};

class unit;

class unitAction{
public:
    unitAction() : type(uType::NORMAL), u(NULL), cible(NULL), frameRestantes(0){

    };
private:
    unit* u;
    uType type;
    unit* cible;
    frame frameRestantes;
};

class unit {
public:
    unit(double x, double y){
        this->x = x;
        this->y = y;
    }
    vector<unitAction> *getPossibleActionVector(){
        return &possibleActions;
    };
//actions
    void move(double x ,double y);
    void attack(unit* b);
    void wait(time_t t);

private:
    double x,y;
    int hp;
    int t_a, t_m;
    uType type;
    vector<unitAction> possibleActions;
};
