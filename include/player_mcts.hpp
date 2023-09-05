#pragma once

#include "game_class.hpp"
#include "player_class.hpp"

class player_mcts final : public virtual_player_class
{

public:
    explicit player_mcts(const player_id id)
        : virtual_player_class(id, player_type::mcts)
    {
    }

    void moves_get(const game_class* game, state_class* state) override;
};