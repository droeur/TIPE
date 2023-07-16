#pragma once

class virtual_player_class;

#include "units.hpp"
#include "state_class.hpp"
#include <iostream>
#include <boost/log/trivial.hpp>

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

    virtual void moves_get(const game_class *game, state_class* state)
    {
        BOOST_LOG_TRIVIAL(error) << "virtual player called";
    }

    [[nodiscard]] player_id player_id_get() const
    {
        return player_id_;
    }
    
};