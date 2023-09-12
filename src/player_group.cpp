#include <vector>
#include <boost/log/trivial.hpp>
#include "player_group.hpp"
#include "mcts.hpp"

using namespace std;

// Player

void player_group::moves_get(const game_class* game, state_class* state)
{
    if (last_squad_check_ <= 0)
    {
        squads_update(state->unit_list_get()[player_id_]);
        last_squad_check_ = last_squad_check_delta_;
    }
    if (last_squad_move_check_ <= 0)
    {
        for (const auto squad:squad_list_)
        {
            squad->moves_generate(game->map_get(), state);
        }
        last_squad_move_check_ = last_squad_move_check_delta_;
    }
    last_squad_move_check_--;
    last_squad_check_--;
}

void player_group::squads_update(const vector<unit_class*>& unit_list)
{
    if (!squad_list_.empty())
    {
        for (const auto s : squad_list_)
            delete s;
        squad_list_.clear();
    }
    if (!unit_list.empty())
    {
        vector<point> point_list;
        for (const auto u : unit_list)
        {
            point p{u, u->q_get(), u->r_get(), -1};
            point_list.push_back(p);
        }
        db_scan db(min_pts_, static_cast<float>(characteristic_size_), point_list);
        db.run();
        point_list = db.m_points;
        int size = 0;
        for (const auto& [u, q, r, cluster_id] : point_list)
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

// Squads

void squad_class::unit_append(unit_class* unit)
{
    unit_list_.push_back(unit);
}

void squad_class::moves_generate(const map_class *map, const state_class *state) const
{
    switch (unit_action action = mcts_->best_action_get(leader_, *state); action.action_type_get())
    {
        case unit_action_id::follow: {
            leader_->follow(action.target_unit_get());
            break;
        }
        case unit_action_id::pick: {
            leader_->pick(dynamic_cast<food_class*>(action.target_unit_get()));
            break;
        }
        case unit_action_id::move: {
            leader_->move(action.position_get().q_get(), action.position_get().r_get());
            break;
        }
        case unit_action_id::attack: {
            leader_->attack(action.target_unit_get());
            break;
        }
        case unit_action_id::wait: {
            leader_->wait(action.time_get());
            break;
        }
        case unit_action_id::error: BOOST_LOG_TRIVIAL(error) << "unknown squad leader action";
    }

    if (leader_->actual_action_get() != nullptr)
    {
        switch (leader_->actual_action_get()->action_type_get())
        {
        case unit_action_id::follow:
        case unit_action_id::pick:
        case unit_action_id::move: {
            for (const auto unit : unit_list_)
            {
                unit->follow(leader_);
            }
            break;
        }
        case unit_action_id::attack: {
            object_abstract_class* enemy = leader_->actual_action_get()->target_unit_get();
            for (const auto unit : unit_list_)
            {
                if (unit->position_get().distance(enemy->position_get(), map) > attack_distance * 2)
                    unit->follow(leader_);
                else
                    unit->attack(enemy);
            }
            break;
        }
        case unit_action_id::wait: {
            for (const auto unit : unit_list_)
            {
                unit->wait(leader_->actual_action_get()->time_get());
            }
            break;
        }
        case unit_action_id::error: BOOST_LOG_TRIVIAL(error) << "unknown squad leader action";
        }
    }
}

unit_class& squad_class::leader_get() const
{
    return *leader_;
}