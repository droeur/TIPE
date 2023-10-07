#include "mcts.hpp"

#include <random>
#include <ctime>

using namespace std;

void mcts::traverse(mcts_node& node, state_class& state)
{
    int play_eval;
    if (node.visits_get() == 0)
    {
        players_[0].moves_get(nullptr, &state);
        players_[1].moves_get(nullptr, &state);
        state.moves_make(map_);
        play_eval = state.evaluate(node.player_get());
    }
}

vector<unit_action> mcts::best_action_calculate(state_class& initial_state, const player_id player)
{
    const clock_t begin = clock();

    root_node_ = mcts_node(nullptr, player);

    for (traversals_ = 0; traversals_ < traversals_max_; traversals_++)
    {
        state_class state(initial_state);
        traverse(root_node_, state);
        clock_t end = clock();
        if (end - begin >= 20)
            break;
    }
    cout << traversals_ << endl;

    //TODO
    vector<unit_action> action_vec;
    for (unit_class& u : initial_state.unit_list_get()[player])
    {
        const auto rand_gen = new std::mt19937{static_cast<unsigned>(time(nullptr)) * 5};
        if (const std::vector<unit_action> possible_actions_vec = initial_state.moves_generate(player, u);
            !possible_actions_vec.empty())
        {
            const std::vector<unit_action>::size_type r = (*rand_gen)() % possible_actions_vec.size();
            action_vec.push_back(possible_actions_vec[r]);
        }
        else
        {
            const unit_action action(&u, unit_action_id::nothing);
            action_vec.push_back(action);
        }
    }
    const clock_t end = clock();
    results.time = end - begin;
    return action_vec;
}
