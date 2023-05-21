#include <iostream>
#include "state.hpp"
#include "game.hpp"
#include "player.hpp"

using namespace std;

/*-state-*/
void state::frame_increment(){
    this->_frame++;
}
frame state::frame_get(){
    return _frame;
}
void state::unitList_add(vector<unit> U){
    this->_U_list.push_back(U);
}
vector<vector<unit>> state::unitList_get(){
    return this->_U_list;
}


void game::play(state &s, vector<player*> P) {
    s.moves_generate();
    for(player* p:P){
        cout << (int)p->getPlayerID() << endl;
        p->getMoves(s, s.possibles_actions_get()[(int)p->getPlayerID()]);
    }
    s.moves_make();
}