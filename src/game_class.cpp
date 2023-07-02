#include "state_class.hpp"
#include "game_class.hpp"
#include "player_class.hpp"

using namespace std;

void game_class::player_set(player_class* p, const player_id id)
{
    if (static_cast<unsigned long long>(id) == players_.size())
        players_.push_back(p);
    else
        std::cerr << "Error: mismatched player id: player:" << id << " vs "
            << players_.size();
}

void game_class::play(state_class *s) const
{
    s->moves_generate();
    for (const auto p : players_)
    {
        s->chosen_actions_set(p->getMoves(s, s->possibles_actions_get()[p->getPlayerID()]), p->getPlayerID());
    }
    s->moves_make();
}

int game_class::winner_check(state_class *s)
{
    vector sums{static_cast<int>(player_number_get()), 0};
    for (base_class& b : *s->base_list_get())
    {
        sums[b.playerId_get()] += b.hp_get();
    }
    for (vector<unit>& u_l : s->unit_list_get())
    {
        for (unit& u : u_l)
        {
            sums[u.player_get()] += u.hp_get();
        }
    }
    if (count(sums.begin(), sums.end(), 0) == sums.size() - 1)
    {
        int index = 0;
        for (const auto i:sums)
        {
            if (i != 0)
                winner_ = index;
            index += 1;
        }
        return true;
    }
    return false;
}

