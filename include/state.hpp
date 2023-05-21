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
    };
    void frame_increment();
    frame frame_get();

    void moves_generate();
    void moves_make();

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
    vector<vector<unitAction>> _possibles_actions; //vector of possible actions
    vector<vector<unitAction>> _choosed_actions; //vector of possible actions
    vector<vector<tile>> _map;
    game *g;
    double fps;
};