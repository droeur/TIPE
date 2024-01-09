#pragma once
#include <vector>
#include "units.hpp"
#include "state_class.hpp"
#include "player_random_total.hpp"

class mcts_result
{
public:
    clock_t time = 0;
    clock_t time_max = 0;
    int traversals = 0;
    int traversals_max = 0;
    std::vector<unit_action> best_actions;
    frame tick_max = 0;
};

class mcts_node
{
    std::vector<mcts_node> children_;
    mcts_node* parent_ = nullptr;
    int visits_ = 0;
    int wins_ = 0;
    double uct_val_ = 0;

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
            state_.options_set(parent_->state_.options_get());
        }
        else
            BOOST_LOG_TRIVIAL(error) << "MCTS : parent null ptr";
    }

    [[nodiscard]]   int visits_get()    const   {   return visits_; }
    void visits_increment(const int visits) { visits_ += visits; }

    [[nodiscard]] int win_get() const { return wins_; }
    void win_increment(const int wins) { wins_ += wins; }

    [[nodiscard]] double uct_val_get() const { return uct_val_; }
    void uct_val_update()
    {
        if (parent_ != nullptr)
        {
            uct_val_ = static_cast<double>(win_get()) / visits_get() +
                       0 * sqrt(log(parent_->visits_get() + 1) / visits_get());
        }
        else
        {
            uct_val_ = static_cast<double>(win_get()) / visits_get();
        }
    }

    [[nodiscard]]   int player_get() const {   return player_; }

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

    double c_parameter_ = sqrt(2);
    int children_parameter_ = 2;

    bool multithreading_;
    int child_depth_;

    player_id max_player_;

    static mcts_node& uct_select(mcts_node& node);
    void expansion(mcts_node& node) const;
    void simulation(mcts_node& node, int& tick_max);
    void simulate_a_thread(mcts_node* child);
    static void back_propagation(mcts_node& node);

    mcts_node& best_node_select();

    mcts_result results_;

    map_class* map_;
    player_rand_total players_[2] = {player_rand_total(0), player_rand_total(1)};


public:
    mcts(const state_class& root_state, const int traversals_max, map_class* map, const player_id max_player,
         const int max_time = 20, const double c_parameter = sqrt(2), const int children_parameter = 2, const bool multi_threading = false, const int child_depth = 1)
        : root_node_(mcts_node(root_state, max_player))
        , max_time_(max_time)
        , traversals_max_(traversals_max)
        , c_parameter_(c_parameter)
        , children_parameter_(children_parameter)
        , multithreading_(multi_threading)
        , child_depth_(child_depth)
        , max_player_(max_player)
        , map_(map)
    {
    }
    std::vector<unit_action> best_action_calculate(const state_class& initial_state, player_id player);
    mcts_result& results_get() { return results_; }
    [[nodiscard]] double c_parameter_get() const { return c_parameter_; }
    [[nodiscard]] int children_parameter_get() const { return children_parameter_; }
    [[nodiscard]] bool multi_threading_get() const { return multithreading_; }
    [[nodiscard]] int child_depth_get() const { return child_depth_; }
};
