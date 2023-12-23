#pragma once

#include <random>
#include "game_class.hpp"
#include "player_class.hpp"

class player_rand final : public virtual_player_class
{
public:
    explicit player_rand(const player_id id)
        : virtual_player_class(id, player_type::random)
    {
    }

    void moves_get(const game_class* game, state_class* state) override;
};