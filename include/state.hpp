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
    state(vector<vector<tile>> map) {
        _frame = 0;
        this->_map = map;
    };
    void frame_increment();
    frame frame_get();

    void moves_generate();
    void moves_make();

    void unitList_add(vector<unit> U);
    vector<vector<unit>> unitList_get();
private:
    frame _frame;
    vector<vector<unit>> _U_list;
    vector<vector<unitAction>> _P_actions;
    vector<vector<tile>> _map;
};