#include <vector>
#include "player_group.hpp"

using namespace std;

vector<unit_action> player_group::moves_get(state_class* s)
{
    std::vector<unit_action> a;
    if (last_squad_check_ >= 0)
    {
        squads_update(s->unit_list_get()[player_id_]);
        last_squad_check_ = last_squad_check_delta_;
    }
    last_squad_check_--;
    return a;
}

void squad_class::unit_append(unit_class* unit)
{
    unit_list_.push_back(unit);
}

void player_group::squads_update(const vector<unit_class*> &unit_list)
{
    if (!squad_list_.empty())
    {
        for (const auto s:squad_list_)
            delete s;
        squad_list_.clear();
    }
    if (!unit_list.empty())
    {
        vector<point> point_list;
        for (const auto u:unit_list)
        {
            point p{u,u->q_get(), u->r_get(), -1};
            point_list.push_back(p);
        }
        dbscan db(min_pts_, characteristic_size_, point_list);
        db.run();
        point_list = db.m_points;
        int size = 0;
        for (const auto &[u, q, r, cluster_id] : point_list)
        {
            if (cluster_id - 1 >= size)
            {
                size++;
            }
        }
        cout << size;
        squad_list_.resize(size);
        for (const auto& [u, q, r, cluster_id] : point_list)
        {
            if (cluster_id - 1 >= 0)
            {
                if (squad_list_[cluster_id - 1] == nullptr)
                {
                    const auto s = new squad_class(u);
                    squad_list_[cluster_id - 1] = s;
                }
                else
                    squad_list_[cluster_id - 1]->unit_append(u);
            }
        }
    }
}
