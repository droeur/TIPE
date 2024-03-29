#pragma once

class virtual_player_class;

enum class player_type
{
    error,
    nothing,
    random_total,
    random_constant,
    mcts,
    group,
    dpf
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
    static std::string player_type_to_string(const player_type type)
    {
        std::string name;
        switch (type)
        {
        case player_type::error: name = "Error"; break;
        case player_type::nothing: name = "Nothing"; break;
        case player_type::group: name = "Group"; break;
        case player_type::mcts: name = "MCTS"; break;
        case player_type::dpf: name = "DPF"; break;
        case player_type::random_constant: name = "Constant Random"; break;
        case player_type::random_total: name = "Total Random"; break;
        }
        return name;
    }
};

class player_nothing final : public virtual_player_class
{
public:
    explicit player_nothing(const player_id id)
        : virtual_player_class(id, player_type::dpf)
    {
    }

    void moves_get(const game_class* game, state_class* state) override {}
};
