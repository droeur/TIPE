#include <random>
#include "player_random.hpp"

using namespace std;

vector<unit_action> player_rand::moves_get(state_class *s){
    const vector<vector<unit_action>> possible_actions_vec = s->moves_generate(player_id_);
    vector<unit_action> chosen_actions;
    const vector<unit_action>::size_type unit_list_size = s->unit_list_get()[player_id_].size();
    vector<unit_action>::size_type unit_index = 0;
    while(unit_index < unit_list_size){
        if(!possible_actions_vec[unit_index].empty()){
            const vector<unit_action>::size_type r = (*rand_gen_)() % possible_actions_vec[unit_index].size();
            unit_action action_temp_chosen = possible_actions_vec[unit_index][r];
            chosen_actions.push_back(action_temp_chosen);
        }
        unit_index++;
    }
    return chosen_actions;
}