#pragma once

#include <vector>

using namespace std;

typedef int frame;

#define ATTACK_COOLDOWN (frame)24
#define ATTACK_DISTANCE 4

#define SPEED_FACTOR 0.5

enum class uType {NORMAL};
enum class uActionID {ERROR, MOVE, ATTACK, WAIT};

class unit;
class unitAction;
#include "object.hpp"
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
    ~unitAction(){
        
    };

    unit* unit_get(){
        return _u;
    }
    uActionID actionType_get(){
        return _actionType;
    }
    unit* targetUnit_get(){
        return _cible;
    }
    position &position_get(){
        return p;
    }
protected:
    unit* _u;
    uActionID _actionType;
    unit* _cible;
    frame _frameRestantes;
    position p;
};

class unit : public object_abstract_class {
public:
    unit(int q, int r, PlayerID id, int hp) : object_abstract_class(q,r){
        t_a = 0;
        t_m = 0;
        actualAction = NULL;
        _joueur = id;
        this->hp = hp;
    }
    void setActualAction(unitAction *action){
        actualAction = action;
    }
    unitAction *getActualAction(){
        return actualAction;
    };
//actions
    void move(double x ,double y);
    void attack(unit* b);
    void wait(time_t t);

    bool canMove() {return t_m == 0 && hp > 0;};
    bool canAttack() {return t_a == 0 && hp > 0;};

    void update_coolDown(){
        if(t_m > 0)
            t_m--;
        else
            t_m = 0;
        if(t_a > 0)
            t_a--;
        else
            t_a = 0;
    }

    int getHP(){
        return hp;
    }
    void setPath(vector<hex_tile*> &path){
        _path = path;
    }
    vector<hex_tile*>* getPath(){
        return &_path;
    }
private:
    PlayerID _joueur;
    int hp;
    int t_a, t_m; // attack cooldown and move cooldown
    uType type;
    unitAction *actualAction;
    vector<hex_tile*> _path;
};
