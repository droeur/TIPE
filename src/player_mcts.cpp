#include "player_mcts.hpp"
#include "mcts.hpp"

using namespace std;

void player_mcts::moves_get(const game_class* game, state_class* state)
{
    if (mcts_ == nullptr)
        mcts_ = new mcts(10, game->map_get());
    const auto size = state->unit_list_get()[player_id_].size();
    for (int unit_index = 0; static_cast<size_t>(unit_index) < size; unit_index++)
    {
        if (const auto unit = state->unit_list_get()[player_id_][unit_index]; unit->can_attack() || unit->can_move())
        {
            switch (unit_action action = mcts_->best_action_calculate(unit, *state, player_id_);
                    action.action_type_get())
            {
            case unit_action_id::attack:
                unit->attack(action.target_unit_get());
                break;
            case unit_action_id::move:
                unit->move(action.position_get().q_get(), action.position_get().r_get());
                break;
            case unit_action_id::wait:
                unit->wait(action.time_get());
                break;
            case unit_action_id::pick:
                unit->pick(dynamic_cast<food_class*>(action.target_unit_get()));
                break;
            case unit_action_id::follow:
                unit->follow(action.target_unit_get());
                break;
            case unit_action_id::error: BOOST_LOG_TRIVIAL(error) << "Unknown action"; break;
            default: break;
            }
        }
    }
}
