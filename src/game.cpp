#include <iostream>
#include "state.hpp"
#include "game.hpp"
#include "player.hpp"

using namespace std;

void game::play(state &s, vector<player*> P) {
    s.moves_generate();
    for(player* p:P){
        s.chosen_actions_set(p->getMoves(s, s.possibles_actions_get()[(int)p->getPlayerID()]), (int)p->getPlayerID());
    }
    s.moves_make();
}

