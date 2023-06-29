#pragma once
#include "player.hpp"

class groupPlayer : public player {
public:
    groupPlayer (const PlayerID id) {
        _playerID = id;
    }
    virtual vector<unitAction> getMoves(state &s, vector<vector<unitAction>> & possibleActionsVec);
protected:

};

class squad
{
public:

};