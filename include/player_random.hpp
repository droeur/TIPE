#pragma once

#include "game.hpp"
#include "player.hpp"

class randPlayer : public player {
public:
    randPlayer (const PlayerID id) {
        _playerID = id;
    }
    virtual void getMoves(state &s, vector<unitAction> & m);
};