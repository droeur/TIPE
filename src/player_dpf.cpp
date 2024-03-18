#include "player_dpf.hpp"

using namespace std;

// joueur attaque les plus puissant
void player_dpf::moves_get(const game_class* game, state_class* state)
{
    player_id player_enemy = game->enemy_player_get(player_id_);
    unit_class* targeted_enemy = &(state->unit_get(player_enemy, 0));
    for(size_t u_index(0); u_index < state->unit_number(player_enemy); u_index++)
    {
        if(state->unit_get(player_enemy, u_index).hp_get()>0 && targeted_enemy->damage_per_frame_get() >= state->unit_get(player_enemy, u_index).damage_per_frame_get())
        {
            targeted_enemy =  &(state->unit_get(player_enemy, u_index));
        }
    }
    cout << targeted_enemy << endl;
    for(size_t u_index(0); u_index < state->unit_number(player_id_); u_index++)
    {
        if(state->unit_get(player_id_, u_index).actual_action_get().action_type_get() == unit_action_id::nothing)
        {
            state->unit_get(player_id_, u_index).attack(*targeted_enemy); 
        }
    }
}
