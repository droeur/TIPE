#pragma once

#include <vector>
#include "player.hpp"
#include "units.hpp"
#include "map.hpp"

using namespace std;

class state {
public:
    state(vector<vector<tile>> map) {
        t = 0;
        this->map = map;
    };
    void time_increment();
    time_t time_get();

    void moves_generate();
    void moves_make();

    void unitList_add(vector<unit> U);
    vector<vector<unit>> unitList_get();
private:
    time_t t;
    vector<vector<unit>> U_list;
    vector<vector<tile>> map;
};