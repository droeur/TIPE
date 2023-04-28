#include <iostream>
#include "game.hpp"

using namespace std;

/*-state-*/
void state::time_update(){
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


void game(state *s, vector<vector<action>(*)(state *s, vector<unit> U)> P) {
    int index = 0;
    s->time_update();
    for(auto p : P){
        p(s, (s->unitList_get()).at(index));
    }
}

vector<action> player(state *s, vector<unit> U){
    vector<action> m;
    return m;
} 