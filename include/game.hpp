#pragma once

#include <vector>
#include "player.hpp"
#include "units.hpp"
#include "state.hpp"

#define NUMBER_OF_PLAYERS 2

using namespace std;

class game{
public:
    void play(state *s, vector<vector<unitAction>(*)(state *s, vector<unit> U)> P);
private:
    player _players[NUMBER_OF_PLAYERS];
    vector<unitAction> scriptMoves[NUMBER_OF_PLAYERS];
};