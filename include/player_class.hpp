#pragma once

class virtual_player_class;

enum class player_type
{
    error,
    random,
    mcts,
    group
};


#include "units.hpp"
#include "state_class.hpp"
#include <iostream>
#include <boost/log/trivial.hpp>


class virtual_player_class
{
protected:
    player_id player_id_;
    player_type player_type_;

public:
    explicit virtual_player_class(const player_id id, const player_type type)
        : player_id_(id)
        , player_type_(type)
    {
    }

    virtual ~virtual_player_class() = default;

    virtual void moves_get(const game_class *game, state_class* state)
    {
        BOOST_LOG_TRIVIAL(error) << "virtual player called";
    }

    [[nodiscard]] player_id player_id_get()     const   {   return player_id_;      }
    [[nodiscard]] player_type player_type_get() const   {   return player_type_;    }
    
};