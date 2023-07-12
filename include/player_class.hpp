#pragma once

#include "units.hpp"
#include "state_class.hpp"
#include <vector>
#include <iostream>

class virtual_player_class
{
protected:
    player_id player_id_;

public:
    explicit virtual_player_class(const player_id id)
        : player_id_(id)
    {
        
    }

    virtual ~virtual_player_class() = default;

    virtual void moves_get(state_class* s)
    {
        std::cout << "Error: base player" << std::endl;
    }

    [[nodiscard]] player_id player_id_get() const
    {
        return player_id_;
    }
    
};