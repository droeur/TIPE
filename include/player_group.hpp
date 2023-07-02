#pragma once
#include "game_class.hpp"
#include "player_class.hpp"

class groupPlayer : public player_class {
public:
    groupPlayer(const player_id id) {
        _playerID = id;
    }
    virtual std::vector<unit_action> getMoves(state_class* s, std::vector<std::vector<unit_action>>& possibleActionsVec);

protected:

};

class squad
{
public:

};