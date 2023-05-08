#include <iostream>
#include "game.hpp"
#include "state.hpp"

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


void game::play(state *s, vector<vector<unitAction>(*)(state *s, vector<unit> U)> P) {
    int index = 0;
    s->moves_generate();
    s->moves_make();
}