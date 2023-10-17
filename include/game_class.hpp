#pragma once
#include "parser.hpp"

constexpr auto number_of_players = 2;

class game_class;

#include "player_class.hpp"

class game_class
{
    std::vector<virtual_player_class*> players_;
    player_id winner_ = -1;
    map_class* map_;
    state_class* state_;
    options_class* options_;

public:
    game_class(map_class* map, state_class* state, options_class* options)
        : map_(map)
        , state_(state)
        , options_(options)
    {}

    void play() const;

    bool winner_check();
    void winner_reset() { winner_ = -1; }
    [[nodiscard]] player_id winner_get() const { return winner_; }

    static player_id enemy_player_get(player_id p);

    void player_set(virtual_player_class* p, player_id id);
    [[nodiscard]] player_type player_type_get(player_id player) const;

    [[nodiscard]] std::vector<virtual_player_class*>::size_type player_number_get() const;

    [[nodiscard]] virtual_player_class& player_get(const player_id id) const  { return *players_[id]; }

    [[nodiscard]] options_class& options_get() const { return *options_; }

    [[nodiscard]] map_class* map_get() const { return map_; }
    [[nodiscard]] state_class* state_get() const { return state_; }
};
