#pragma once

#include "types.hpp"
#include "units.hpp"
#include "state.hpp"
#include <vector>

class player {
public:
    virtual void getMoves(state &s, vector<unitAction> &m){
        cout << "ERROR : base player" << endl;
    };
    PlayerID getPlayerID(){
        return _playerID;
    };
protected:
    PlayerID _playerID;
};