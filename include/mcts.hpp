#pragma once
#include <vector>
#include "units.hpp"
#include "state_class.hpp"
#include "player_random.hpp"

class mcts_result
{
public:
    clock_t time = 0;
    int traversals_;
    std::vector<unit_action> best_actions;
};

class mcts_node
{
    std::vector<mcts_node> children_;
    mcts_node* parent_ = nullptr;
    int visits_ = 0;
    int wins_ = 0;
    int uct_ = 0;

    player_id player_ = -1;

public:
    mcts_node() = default;

    mcts_node(mcts_node* parent, const player_id player)
        : parent_(parent)
        , player_(player)
    {
    }

    [[nodiscard]]   int visits_get()    const   {   return visits_; }
                    void visits_increment()     {   visits_++; }

    [[nodiscard]]   int player_get()    const   {   return player_; }
};

class mcts
{
    mcts_node root_node_;
    clock_t max_time_ = 40;

    int traversals_max_;
    int traversals_ = 0;

    void traverse(mcts_node& node, state_class& state);

    mcts_result results;

    map_class* map_;
    player_rand players_[2] = {player_rand(0), player_rand(1)};

public:
    mcts(const int traversals_max, map_class* map)
        : traversals_max_(traversals_max)
        , map_(map)
    {
    }

    unit_action best_action_calculate(unit_class& unit, const state_class& initial_state, player_id player);
};