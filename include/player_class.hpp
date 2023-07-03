#pragma once

#include "units.hpp"
#include <vector>

class virtual_player_class {
public:
    explicit virtual_player_class(const player_id id)
        : player_id_(id)
    {
        
    }

    virtual ~virtual_player_class() = default;

    virtual std::vector<unit_action> moves_get(state_class* s)
    {
        std::vector<unit_action> v;
        std::cout << "ERROR : base player" << std::endl;
        return v;
    }

    [[nodiscard]] player_id player_id_get() const
    {
        return player_id_;
    }
    
protected:
    player_id player_id_;
};