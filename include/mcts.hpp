#pragma once


template <GameState State>
typename State::Move compute_move(const State root_state, const ComputeOptions options)
{
    using namespace std;

    // Will support more players later.
    minimum_core_assert(root_state.player_to_move == 1 || root_state.player_to_move == 2);

    auto moves = root_state.get_moves();
    minimum_core_assert(moves.size() > 0);
    if (moves.size() == 1)
    {
        return moves[0];
    }

    double start_time = core::wall_time();

    // Start all jobs to compute trees.
    vector<future<unique_ptr<Node<State>>>> root_futures;
    ComputeOptions job_options = options;
    job_options.verbose = false;
    for (int t = 0; t < options.number_of_threads; ++t)
    {
        auto func = [t, &root_state, &job_options]() -> std::unique_ptr<Node<State>> {
            return compute_tree(root_state, job_options, 1012411 * t + 12515);
        };

        root_futures.push_back(std::async(std::launch::async, func));
    }

    // Collect the results.
    vector<unique_ptr<Node<State>>> roots;
    for (int t = 0; t < options.number_of_threads; ++t)
    {
        roots.push_back(std::move(root_futures[t].get()));
    }

    // Merge the children of all root nodes.
    map<typename State::Move, int> visits;
    map<typename State::Move, double> wins;
    long long games_played = 0;
    for (int t = 0; t < options.number_of_threads; ++t)
    {
        auto root = roots[t].get();
        games_played += root->visits;
        for (auto child = root->children.cbegin(); child != root->children.cend(); ++child)
        {
            visits[(*child)->move] += (*child)->visits;
            wins[(*child)->move] += (*child)->wins;
        }
    }

    // Find the node with the most visits.
    double best_score = -1;
    typename State::Move best_move = typename State::Move();
    for (auto itr : visits)
    {
        auto move = itr.first;
        double v = itr.second;
        double w = wins[move];
        // Expected success rate assuming a uniform prior (Beta(1, 1)).
        // https://en.wikipedia.org/wiki/Beta_distribution
        double expected_success_rate = (w + 1) / (v + 2);
        if (expected_success_rate > best_score)
        {
            best_move = move;
            best_score = expected_success_rate;
        }
    }

    return best_move;
}