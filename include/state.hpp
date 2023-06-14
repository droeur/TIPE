#pragma once

#include <vector>

class state;

#include "units.hpp"
#include "hex_map.hpp"
#include "game.hpp"
#include "food.hpp"
#include "base.hpp"

using namespace std;

typedef int frame;

class state {
public:
    state(map_class* map, game* g) {
        _frame = 0;
        this->_map = map;
        this->g = g;
        vector<vector<unitAction>> possibles_actionList1;
        _possibles_actions[0] = possibles_actionList1;
        vector<vector<unitAction>> possibles_actionList2;
        _possibles_actions[1] = possibles_actionList2;
    };
    void frame_increment();
    frame frame_get();

    void moves_generate();
    void moves_make();

    vector<vector<unitAction>>* possibles_actions_get(){
        return _possibles_actions;
    }

    void choosed_actions_set(vector<unitAction> choosed, PlayerID id){
        _choosed_actions[id].clear();
        _choosed_actions[id] = choosed;
    }
    vector<unitAction> choosed_actions_get(PlayerID id){
        return _choosed_actions[id];
    }

    void unitList_add(vector<unit> U);
    vector<vector<unit>> unitList_get();
    void unit_append(unit &u, PlayerID joueur);

    void food_append(food_class &f){
        _food_list.push_back(f);
    };
    vector<food_class>* foodList_get(){
        return &_food_list;
    }
    void base_append(base_class &f){
        _base_list.push_back(f);
    };
    vector<base_class>* baseList_get(){
        return &_base_list;
    }

    void fps_set(double f){
        fps = f;
    }
    double fps_get(){
        return fps;
    }
    map_class *map_get(){
        return _map;
    }
private:
    frame _frame;
    vector<vector<unit>> _U_list;
    vector<vector<unitAction>> _possibles_actions[NUMBER_OF_PLAYERS]; //vector of possible actions
    vector<unitAction> _choosed_actions[NUMBER_OF_PLAYERS]; //vector of possible actions
    map_class *_map;
    game *g;
    double fps;
    vector<food_class> _food_list;
    vector<base_class> _base_list;
};