#pragma once

#include "types.hpp"
#include "units.hpp"
#include "state.hpp"
#include <vector>

class player {
public:
    void getMoves(state &s, vector<unitAction> & m);
protected:
    PlayerID _playerID;
};