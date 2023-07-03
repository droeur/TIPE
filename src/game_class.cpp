#include "state_class.hpp"
#include "game_class.hpp"
#include "player_class.hpp"

using namespace std;

void game_class::player_set(virtual_player_class* p, const player_id id)
{
    if (static_cast<unsigned long long>(id) == players_.size())
        players_.push_back(p);
    else
        std::cerr << "Error: mismatched player id: player:" << id << " vs "
            << players_.size();
}

void game_class::play() const
{
    for (const auto p : players_)
    {
        state_->chosen_actions_set(p->moves_get(state_), p->player_id_get());
    }
    state_->moves_make(map_);
}

bool game_class::winner_check()
{
    vector<unsigned long long> sums(player_number_get(), 0);
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
    /*
    {
        vector<unsigned long long> sums(player_number_get(), 0);
        for (const auto b : *state_->base_list_get())
        {
            sums[b->player_id_get()] += b->hp_get();
        }
        if (count(sums.begin(), sums.end(), 0) == sums.size() - 1)
        {
            int index = 0;
            for (const auto i : sums)
            {
                if (i != 0)
                {
                    winner_ = index;
                }
                index += 1;
            }
        }
    }
    {
        vector<unsigned long long> sums(player_number_get(), 0);
        for (const auto &u_l : state_->unit_list_get())
        {
            for (const auto u : u_l)
            {
                sums[u->player_get()] += u->hp_get();
            }
        }
        if (count(sums.begin(), sums.end(), 0) == sums.size() - 1)
        {
            int index = 0;
            for (const auto i:sums)
            {
                if (i != 0 && winner_ == -1)
                {
                    winner_ = index;
                }
                else if (i != 0)
                {
                    winner_ = 2;
                }
                index += 1;
            }
        }
    }*/
    return winner_ != -1;
}

