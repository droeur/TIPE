#include <random>
#include "player_random.hpp"

using namespace std;

void player_rand::moves_get(state_class *s){
    const vector<vector<unit_action>> possible_actions_vec = s->moves_generate(player_id_);
    const vector<unit_action>::size_type unit_list_size = s->unit_list_get()[player_id_].size();
    vector<unit_action>::size_type unit_index = 0;
    while(unit_index < unit_list_size){
        if(!possible_actions_vec[unit_index].empty()){
            const vector<unit_action>::size_type r = (*rand_gen_)() % possible_actions_vec[unit_index].size();
            const auto action_temp_chosen = new unit_action(possible_actions_vec[unit_index][r]);
            s->unit_list_get()[player_id_][unit_index]->actual_action_set(action_temp_chosen);
        }
        unit_index++;
    }
}