#pragma once

#include <vector>

class state;

#include "units.hpp"
#include "map.hpp"
#include "game.hpp"

using namespace std;

typedef int frame;

class state {
public:
    state(vector<vector<tile>> map, game* g) {
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

    void unitList_add(vector<unit> U);
    vector<vector<unit>> unitList_get();

    void fps_set(double f){
        fps = f;
    }
    double fps_get(){
        return fps;
    }
    
private:
    frame _frame;
    vector<vector<unit>> _U_list;
    vector<vector<unitAction>> _possibles_actions[NUMBER_OF_PLAYERS]; //vector of possible actions
    vector<unitAction> _choosed_actions[NUMBER_OF_PLAYERS]; //vector of possible actions
    vector<vector<tile>> _map;
    game *g;
    double fps;
};