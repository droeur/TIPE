#pragma once

#include "game.hpp"
#include "player.hpp"

class randPlayer : public player {
public:
    randPlayer (const PlayerID id) {
        _playerID = id;
    }
    virtual vector<unitAction> getMoves(state &s, vector<vector<unitAction>> & possibleActionsVec);
};