#pragma once

#include <vector>
#include <iostream>

#define NUMBER_OF_PLAYERS 2

using namespace std;

class game;

#include "types.hpp"
#include "state.hpp"
#include "player.hpp"
#include "units.hpp"

class game{
public:
    static void play(state& s, vector<player*>& P);
    player_id getEnnemy(player_id p){
        return p == 0;
    };
private:
    player _players[NUMBER_OF_PLAYERS];
    vector<unit_action> scriptMoves[NUMBER_OF_PLAYERS];
};