#include "player_mcts.hpp"
#include "mcts.hpp"

using namespace std;

void player_mcts::player_mcts_init(const game_class* game, int time, int traversals_max)
{
    mcts_ = new mcts(traversals_max, game->map_get(), player_id_, time);
}

void player_mcts::moves_get(const game_class* game, state_class* state)
{
    if (mcts_ == nullptr)
        BOOST_LOG_TRIVIAL(error) << "MCTS not initizalized";
    const auto size = state->unit_list_get()[player_id_].size();

    vector<unit_action> action_vec = mcts_->best_action_calculate(*state, player_id_);
    for (unit_action& action: action_vec)
    {
        switch (action.action_type_get())
        {
            case unit_action_id::attack:
            state->unit_get(player_id_, action.unit_get()->id_get()).attack(
                    state->object_get(action.target_type_get(), action.target_player_get(), action.target_id_get()));
                break;
            case unit_action_id::move:
                state->unit_get(player_id_, action.unit_get()->id_get()).move(action.position_get().q_get(), action.position_get().r_get());
                break;
            case unit_action_id::wait:
                state->unit_get(player_id_, action.unit_get()->id_get()).wait(action.time_get());
                break;
            case unit_action_id::pick:
                state->unit_get(player_id_, action.unit_get()->id_get()).pick(dynamic_cast<food_class&>(
                    state->object_get(action.target_type_get(), action.target_player_get(), action.target_id_get())));
                break;
            case unit_action_id::follow:
                state->unit_get(player_id_, action.unit_get()->id_get()).follow(state->object_get(action.target_type_get(), action.target_player_get(),
                                              action.target_id_get()));
                break;
            case unit_action_id::error: BOOST_LOG_TRIVIAL(error) << "Unknown action"; break;
            case unit_action_id::nothing:
            default: break;
        }
    }
}
