#include "mcts.hpp"

#include <random>
#include <ctime>

using namespace std;

mcts_node& mcts::uct_select(mcts_node& node) const
{
    if (node.children_get().empty())
        return node;

    mcts_node* selected_node = &node;
    while (!selected_node->children_get().empty())
    {
        int best_node_index = 0;
        double best_node_uct = 0;
        int index = 0;
        for (const auto& child : selected_node->children_get())
        {
            const double uct_val = static_cast<double>(child.win_get()) / child.visits_get() +
                                   sqrt(2) * sqrt(log(selected_node->visits_get()) / child.visits_get());
            if (uct_val > best_node_uct)
            {
                best_node_uct = uct_val;
                best_node_index = index;
            }
            cout << selected_node->children_get().size()
                 << "  ";
            index++;
        }
        selected_node = &selected_node->children_get()[best_node_index];
    }
    return *selected_node;
}

void mcts::expansion(mcts_node& node)
{
    vector<mcts_node>& children = node.children_get();
    mcts_node new_child(&node, node.player_get());
    children.push_back(new_child);
    new_child = mcts_node(&node, node.player_get());
    children.push_back(new_child);
    new_child = mcts_node(&node, node.player_get());
    children.push_back(new_child);
}

void mcts::simulation(mcts_node& node)
{
    //TODO
    state_class state = node.state_get();
    for (auto& child : node.children_get())
    {
        players_[0].moves_get(nullptr, &state);
        players_[1].moves_get(nullptr, &state);
        child.action_vec_set(state.action_vec_get(max_player_));
        state.moves_make(map_);
        const int play_eval = state.evaluate(child.player_get());
        node.visits_increment();
    }
}

void mcts::back_propagation(const mcts_node& node)
{
    mcts_node* parent_node = node.parent_get();
    while (parent_node != nullptr)
    {
        parent_node->visits_increment();
        if (node.win_get() != 0)
            parent_node->win_increment();
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

    root_node_ = mcts_node(initial_state, player);

    for (traversals_ = 0; traversals_ < traversals_max_; traversals_++)
    {
        mcts_node &node = uct_select(root_node_);
        expansion(node);
        simulation(node);
        back_propagation(node.children_get().back());
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
    return chosen_action;
}
