#include "mcts.hpp"
#include "game_class.hpp"

#include <ctime>
#include <thread>

using namespace std;

player_id mcts_node::player_to_move_next(mcts_node* parent)
{
    return game_class::enemy_player_get(parent->player_to_move_);
}

mcts_node& mcts::uct_select(mcts_node& node)
{
    if (node.children_get().empty())
        return node;
    
    mcts_node* selected_node = &node;
    while (!selected_node->children_get().empty())
    {
        int best_node_index = 0;
        double best_node_uct = 0;
        int index = 0;
        for (auto& child : selected_node->children_get())
        {
            child.uct_val_update();
            if (child.uct_val_get() > best_node_uct)
            {
                best_node_uct = child.uct_val_get();
                best_node_index = index;
            }
            index++;
        }
        selected_node = &selected_node->children_get()[best_node_index];
    }
    return *selected_node;
}

void mcts::expansion(mcts_node& node) const
{
    vector<mcts_node>& children = node.children_get();
    for (int i = 0; i < children_parameter_; i++)
    {
        children.emplace_back(&node, node.player_get());
    }
}
/*
void mcts::simulation(mcts_node& node, int& tick_max)
{
    for (auto& child : node.children_get())
    {
        state_class& state = child.state_get();
        players_[0].moves_get(nullptr, &state);
        players_[1].moves_get(nullptr, &state);
        child.action_vec_set(state.action_vec_get(max_player_));
        for (int i = 0; i < child_depth_; i++)
            state.moves_make(map_);
        if (state.evaluate(max_player_) > 0)
            child.win_increment(1);
        child.visits_increment(1);
        child.uct_val_update();
        tick_max = max(state.frame_get(), tick_max);
    }
}
*/


void mcts::simulation(mcts_node& node, int& tick_max)
{
    vector<thread> th_list;
    for (auto& child : node.children_get())
    {
        th_list.emplace_back(& mcts::simulate_a_thread, this, &child);
    }
    for (auto& th : th_list)
    {
        th.join();
    }
    for (auto& child : node.children_get())
    {
        tick_max = max(child.state_get().frame_get(), tick_max);
    }
}

void mcts::simulate_a_thread(mcts_node* child)
{
    state_class& state = child->state_get();
    players_[0].moves_get(nullptr, &state); //ici on ajoute les actions de chaque joueur
    players_[1].moves_get(nullptr, &state);
    child->action_vec_set(state.action_vec_get(max_player_));
    for (int i = 0; i < child_depth_; i++)
        state.moves_make(map_);
    if (state.evaluate(max_player_) > 0)
        child->win_increment(1);
    child->visits_increment(1);
    child->uct_val_update();
}

void mcts::back_propagation(mcts_node& node)
{
    int child_wins = 0;
    int child_visits = 0;
    child_wins += node.win_get() * static_cast<int>(node.children_get().size());
    child_visits += static_cast<int>(node.children_get().size());
    mcts_node* parent_node = node.parent_get();
    while (parent_node != nullptr)
    {
        parent_node->win_increment(child_wins);
        parent_node->visits_increment(child_visits);
        parent_node = parent_node->parent_get();
    }
}

mcts_node& mcts::best_node_select()
{
    mcts_node* best_node = root_node_.children_get().data();
    for (auto& child : root_node_.children_get())
    {
        if (child.visits_get() != 0 && child.win_get() / child.visits_get() > best_node->win_get() / best_node->visits_get())
            best_node = &child;
    }
    return *best_node;
}



vector<unit_action> mcts::best_action_calculate(const state_class& initial_state, const player_id player)
{
    const clock_t begin = clock();
    frame tick_max = 0;
    root_node_ = mcts_node(initial_state, player);

    for (traversals_ = 0; traversals_ < traversals_max_; traversals_++)
    {
        mcts_node &node = uct_select(root_node_);
        expansion(node);
        simulation(node, tick_max);
        back_propagation(node);
        if (const clock_t end = clock(); end - begin >= max_time_)
            break;
    }

    if (root_node_.children_get().empty())
    {
        vector<unit_action> empty;
        return empty;
    }
    mcts_node &best_node = best_node_select();
    vector<unit_action>& chosen_action = best_node.action_vec_get();
    const clock_t end = clock();
    results_.time = end - begin;
    results_.time_max = max_time_;
    results_.traversals = traversals_;
    results_.traversals_max = traversals_max_;
    results_.tick_max = tick_max;
    return chosen_action;
}
