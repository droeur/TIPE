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

unit_action mcts::best_action_calculate(unit_class* unit, const state_class& initial_state, player_id player)
{
    const clock_t begin = clock();
    const player_id id = unit->player_get();

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

    const auto rand_gen = new std::mt19937{static_cast<unsigned>(time(nullptr)) * 5};
    if (const std::vector<unit_action> possible_actions_vec = initial_state.moves_generate(id, unit);
        !possible_actions_vec.empty())
    {
        const std::vector<unit_action>::size_type r = (*rand_gen)() % possible_actions_vec.size();
        return possible_actions_vec[r];
    }
    const unit_action action(unit, unit_action_id::nothing);
    const clock_t end = clock();
    results.time = end - begin;
    return action;
}
