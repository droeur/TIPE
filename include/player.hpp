#pragma once

#include "types.hpp"
#include "units.hpp"
#include "state.hpp"
#include <vector>

class player {
public:
    virtual vector<unitAction> getMoves(state &s, vector<vector<unitAction>> & possibleActionsVec){
        vector<unitAction> v;
        cout << "ERROR : base player" << endl;
        return v;
    };
    PlayerID getPlayerID(){
        return _playerID;
    };
protected:
    PlayerID _playerID;
};