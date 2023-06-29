#pragma once

#include "units.hpp"
#include <vector>

class player {
public:
    virtual std::vector<unit_action> getMoves(state& s, std::vector<std::vector<unit_action>>& possibleActionsVec)
    {
        std::vector<unit_action> v;
        std::cout << "ERROR : base player" << std::endl;
        return v;
    };
    player_id getPlayerID(){
        return _playerID;
    };
protected:
    player_id _playerID;
};