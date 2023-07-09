#include "state_class.hpp"
#include "game_class.hpp"
#include "player_class.hpp"

using namespace std;

player_id game_class::enemy_player_get(const player_id p)
{
    return p == 0;
}

void game_class::player_set(virtual_player_class* p, const player_id id)
{
    if (static_cast<unsigned long long>(id) == players_.size())
        players_.push_back(p);
    else
        std::cerr << "Error: mismatched player id: player:" << id << " vs "
            << players_.size();
}

std::vector<virtual_player_class*>::size_type game_class::player_number_get() const
{
    return players_.size();
}

void game_class::play() const
{
    for (const auto p : players_)
    {
        p->moves_get(state_);
    }
    state_->moves_make(map_);
}

bool game_class::winner_check()
{
    vector<unsigned long long> sums(player_number_get()*2, 0);
    for (const auto& u_l : state_->unit_list_get())
    {
        for (const auto u : u_l)
        {
            sums[u->player_get()] += u->hp_get();
        }
    }
    if (sums[0] == 0 && sums[1] == 0)
        winner_ = 2;
    else if (sums[0] == 0)
        winner_ = 1;
    else if (sums[1] == 0)
        winner_ = 0;

    for (const auto& b : *state_->base_list_get())
    {
        sums[b->player_get() + 2] += b->hp_get();
    }
    if ((sums[2] == 0 && sums[3] == 0) || ((sums[2] == 0 || sums[3] == 0) && winner_ != -1))
        winner_ = 2;
    else if (sums[2] == 0)
        winner_ = 1;
    else if (sums[3] == 0)
        winner_ = 0;
    return winner_ != -1;
}

