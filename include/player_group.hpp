#pragma once
#include "game_class.hpp"
#include "player_class.hpp"

class player_group final : public virtual_player_class {
public:
    explicit player_group(const player_id id)
        : virtual_player_class(id)
    {
        
    }

    std::vector<unit_action> moves_get(state_class* s) override;

protected:

};

class squad
{
public:

};