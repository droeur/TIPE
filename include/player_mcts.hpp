#pragma once

#include "game_class.hpp"
#include "mcts.hpp"
#include "player_class.hpp"

class player_mcts final : public virtual_player_class
{
    mcts *mcts_ = nullptr;

public:
    explicit player_mcts(const player_id id)
        : virtual_player_class(id, player_type::mcts)
    {
    }
    void player_mcts_init(const game_class* game, const int time, const int traversals_max, const double c_parameter, const int
                          children_parameter, const bool multi_thread, const int child_depth);
    mcts& mcts_get() const { return *mcts_; }
    void moves_get(const game_class* game, state_class* state) override;
};