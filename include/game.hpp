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
    void play(state &s, vector<player*> P);
    PlayerID getEnnemy(PlayerID p){
        return p == 0;
    };
private:
    player _players[NUMBER_OF_PLAYERS];
    vector<unitAction> scriptMoves[NUMBER_OF_PLAYERS];
};