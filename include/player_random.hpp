#pragma once

#include "game.hpp"
#include "player.hpp"

class randPlayer : player {
public:
    randPlayer (const PlayerID id) {
        _playerID = id;
    }
    void getMoves(state &s, vector<unitAction> & m);
};