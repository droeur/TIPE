#pragma once

#include <vector>

using namespace std;

typedef int frame;

#define ATTACK_COOLDOWN 24

enum class uType {NORMAL};
enum class uActionID {ERROR, MOVE, ATTACK, WAIT};

class unit;
class unitAction;
#include "game.hpp"
#include "position.hpp"
#include "player.hpp"

class unitAction{
public:
    unitAction(unit* u, uActionID type, unit* cible, frame frameRestantes) : 
        _u(u), 
        _actionType(type), 
        _cible(cible),
        _frameRestantes(frameRestantes){

    };
protected:
    unit* _u;
    uActionID _actionType;
    unit* _cible;
    frame _frameRestantes;
    double x,y;
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

    bool canMove() {return t_a == 0;};
    bool canAttack() {return t_m == 0;};

private:
    PlayerID joueur;
    double x,y;
    int hp;
    int t_a, t_m; // attack cooldown and move cooldown
    uType type;
    vector<unitAction> possibleActions;
};
