#include <random>
#include "player_random.hpp"

using namespace std;

void player_rand::moves_get(const game_class* game, state_class* state)
{
    const vector<vector<unit_action>> possible_actions_vec = state->moves_generate(player_id_);
    const vector<unit_action>::size_type unit_list_size = state->unit_list_get()[player_id_].size();
    vector<unit_action>::size_type unit_index = 0;
    while(unit_index < unit_list_size){
        if (!possible_actions_vec[unit_index].empty() )
        {
            const vector<unit_action>::size_type r = (*rand_gen_)() % possible_actions_vec[unit_index].size();
            switch (const auto action_temp_chosen = new unit_action(possible_actions_vec[unit_index][r]); action_temp_chosen->action_type_get())
            {
            case unit_action_id::attack: 
                state->unit_list_get()[player_id_][unit_index].attack(
                    state->object_get( action_temp_chosen->target_type_get(), 
                                            action_temp_chosen->target_player_get(),
                                            action_temp_chosen->target_id_get()));
                break;
            case unit_action_id::move:
                state->unit_list_get()[player_id_][unit_index].move(action_temp_chosen->position_get().q_get(),
                                                                 action_temp_chosen->position_get().r_get());
                break;
            case unit_action_id::wait:
                state->unit_list_get()[player_id_][unit_index].wait(action_temp_chosen->time_get());
                break;
            case unit_action_id::pick:
                state->unit_list_get()[player_id_][unit_index].pick(dynamic_cast<food_class&>(
                    state->object_get(action_temp_chosen->target_type_get(), action_temp_chosen->target_player_get(),
                                      action_temp_chosen->target_id_get())));
                break;
            case unit_action_id::follow:
                state->unit_list_get()[player_id_][unit_index].follow(
                    state->object_get(action_temp_chosen->target_type_get(), action_temp_chosen->target_player_get(),
                                      action_temp_chosen->target_id_get()));
                break;
            case unit_action_id::error: BOOST_LOG_TRIVIAL(error) << "Unknown action";
            }
        }
        unit_index++;
    }
}