#pragma once

constexpr auto number_of_players = 2;

class game_class;

#include "player_class.hpp"

class game_class{
public:
    game_class() = default;

    void play(state_class* s) const;

    int winner_check(state_class* s);

    static player_id enemy_player_get(const player_id p){
        return p == 0;
    }

    void player_set(player_class *p, player_id id);

    [[nodiscard]] std::vector<player_class*>::size_type player_number_get() const
    {
        return players_.size();
    }

private:
    std::vector<player_class*> players_;
    player_id winner_ = -1;
};