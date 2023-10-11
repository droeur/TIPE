#pragma once
#include <vector>
#include "units.hpp"
#include "state_class.hpp"
#include "player_random.hpp"

class mcts_result
{
public:
    clock_t time = 0;
    clock_t time_max = 0;
    int traversals = 0;
    int traversals_max = 0;
    std::vector<unit_action> best_actions;
};

class mcts_node
{
    std::vector<mcts_node> children_;
    mcts_node* parent_ = nullptr;
    int visits_ = 0;
    int wins_ = 0;

    bool is_max_;

    state_class state_;
    player_id player_ = -1;

    std::vector<unit_action> action_vec_;

public:
    mcts_node(state_class state, const player_id player)
        : state_(std::move(state))
        , player_(player)
    {
        is_max_ = true;
    }
    mcts_node(mcts_node* parent, const player_id player)
        : parent_(parent)
        , player_(player)
    {
        if (parent != nullptr)
        {
            is_max_ = !parent->is_max_;
            state_ = parent_->state_;
        }
        else
            BOOST_LOG_TRIVIAL(error) << "MCTS : parent null ptr";
    }

    [[nodiscard]]   int visits_get()    const   {   return visits_; }
                    void visits_increment()     {   visits_++; }

    [[nodiscard]] int win_get() const { return wins_; }
                    void win_increment() { wins_++; }

    [[nodiscard]]   int player_get()    const   {   return player_; }

    std::vector<mcts_node>& children_get() { return children_; }

    [[nodiscard]] mcts_node* parent_get() const { return parent_; }

    void is_max_set(const bool max) { is_max_ = max; }
    [[nodiscard]] bool is_max_get() const { return is_max_; }

    [[nodiscard]] state_class& state_get() { return state_; }

    void action_vec_set(const std::vector<unit_action>& action_vec) { action_vec_ = action_vec; }
    std::vector<unit_action>& action_vec_get() { return action_vec_; }
};

class mcts
{
    mcts_node root_node_;
    clock_t max_time_ = 20;

    int traversals_max_;
    int traversals_ = 0;

    player_id max_player_;

    mcts_node& uct_select(mcts_node& node) const;
    static void expansion(mcts_node& node);
    void simulation(mcts_node& node);
    static void back_propagation(const mcts_node& node);

    mcts_node& best_node_select();

    mcts_result results_;

    map_class* map_;
    player_rand players_[2] = {player_rand(0), player_rand(1)};


public:
    mcts(const int traversals_max, map_class* map, const player_id max_player, int max_time=20)
        : max_time_(max_time)
        , root_node_(mcts_node(nullptr, max_player))
        , traversals_max_(traversals_max)
        , max_player_(max_player)
        , map_(map)
    {
    }
    std::vector<unit_action> best_action_calculate(const state_class& initial_state, const player_id player);
    mcts_result& results_get() { return results_; }
};