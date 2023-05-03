#include <iostream>
#include "game.hpp"
#include "state.hpp"

using namespace std;

/*-state-*/
void state::time_increment(){
    this->t++;
}
time_t state::time_get(){
    return t;
}
void state::unitList_add(vector<unit> U){
    this->U_list.push_back(U);
}
vector<vector<unit>> state::unitList_get(){
    return this->U_list;
}


void game::play(state *s, vector<vector<unitAction>(*)(state *s, vector<unit> U)> P) {
    int index = 0;
    s->moves_generate();
    s->moves_make();
}