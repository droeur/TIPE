#pragma once

#include "units.hpp"
#include "state_class.hpp"

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

    int traversals_max_;
    int traversals_;

    void traverse(mcts_node& node, state_class& state);

public:
    mcts(const int traversals_max)
        : traversals_max_(traversals_max)
    {
    }

    unit_action best_action_get(unit_class* unit, const state_class& initial_state);
};