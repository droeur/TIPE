#pragma once

#include <random>
#include "game_class.hpp"
#include "player_class.hpp"

class player_rand final : public virtual_player_class {
public:
    explicit player_rand(const player_id id)
        : virtual_player_class(id)
    {
        rand_gen_ = new std::mt19937{static_cast<unsigned> (time(nullptr))*5};
    }

    std::vector<unit_action> moves_get(state_class* s) override;
private:
    std::mt19937 *rand_gen_;
};