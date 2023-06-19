#pragma once

#include <vector>

using namespace std;

typedef int frame;

#define ATTACK_COOLDOWN (frame)24
#define ATTACK_DISTANCE 4

#define MOVE_COOLDOWN (frame)24

#define SPEED_FACTOR 0.5

#define UNIT_HP 10

enum class uType {NORMAL};
enum class uActionID {ERROR, MOVE, ATTACK, WAIT, PICK};

class unit;
class unitAction;
#include "object.hpp"
#include "game.hpp"
#include "position.hpp"
#include "player.hpp"

class unitAction{
public:
    unitAction(unit* u, uActionID type, object_abstract_class* cible, frame frameRestantes) : 
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
    object_abstract_class* targetUnit_get(){
        return _cible;
    }
    position &position_get(){
        return p;
    }
protected:
    unit* _u;
    uActionID _actionType;
    object_abstract_class* _cible;
    frame _frameRestantes;
    position p;
};




class unit : public object_abstract_class {
public:
    unit(int q, int r, PlayerID id, int hp) : object_abstract_class(q,r, hp, id){
        t_a = 0;
        t_m = 0;
        actualAction = NULL;
    }
    void setActualAction(unitAction *action){
        actualAction = action;
    }
    unitAction *getActualAction(){
        return actualAction;
    }
//actions
    void move(double x ,double y);
    void attack(object_abstract_class* b);
    void wait(time_t t);

    bool canMove() {return t_m == 0 && _HP > 0;};
    bool canAttack() {return t_a == 0 && _HP > 0;};

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

    void setPath(vector<hex_tile*> &path){
        cout << this << " : " << path.size() << endl;
        _path = path;
    }
    vector<hex_tile*>* getPath(){
        return &_path;
    }
private:
    int t_a, t_m; // attack cooldown and move cooldown
    uType type;
    unitAction *actualAction;
    vector<hex_tile*> _path;
};
