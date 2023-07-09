#pragma once

constexpr auto number_of_players = 2;

class game_class;

#include "player_class.hpp"

class game_class
{
    std::vector<virtual_player_class*> players_;
    player_id winner_ = -1;
    map_class* map_;
    state_class* state_;

public:
    game_class(map_class* map, state_class* state)
        : map_(map), state_(state)
    {}

    void play() const;

    bool winner_check();
    [[nodiscard]] player_id winner_get() const { return winner_; }

    static player_id enemy_player_get(player_id p);

    void player_set(virtual_player_class* p, player_id id);

    [[nodiscard]] std::vector<virtual_player_class*>::size_type player_number_get() const;

    [[nodiscard]] map_class* map_get() const { return map_; }
    [[nodiscard]] state_class* state_get() const { return state_; }
};