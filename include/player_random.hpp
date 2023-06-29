#pragma once

#include "game.hpp"
#include "player.hpp"

class randPlayer : public player {
public:
    randPlayer (const player_id id) {
        _playerID = id;
    }
    virtual vector<unit_action> getMoves(state &s, vector<vector<unit_action>> & possibleActionsVec);
};