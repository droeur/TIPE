#include <vector>
#include <boost/log/trivial.hpp>
#include "player_group.hpp"
#include "mcts.hpp"

using namespace std;

// Player

void player_group::moves_get(const game_class* game, state_class* state)
{
    if (map_ != nullptr)
        map_ = game->map_get();

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

void player_group::squads_update(vector<unit_class>& unit_list)
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
        for (auto& u : unit_list)
        {
            point p{&u, u.q_get(), u.r_get(), -1};
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
                    const auto s = new squad_class(*u, map_);
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

void squad_class::moves_generate(const map_class *map, state_class *state) const
{
    switch (unit_action action = mcts_->best_action_calculate(*state, leader_.player_get())[leader_.id_get()];
            action.action_type_get())
    {
        case unit_action_id::follow: {
        leader_.follow(state->object_get(action.target_type_get(), action.target_player_get(), action.target_id_get()));
            break;
        }
        case unit_action_id::pick: {
            leader_.pick(dynamic_cast<food_class&>(
                state->object_get(action.target_type_get(), action.target_player_get(), action.target_id_get())));
            break;
        }
        case unit_action_id::move: {
            leader_.move(action.position_get().q_get(), action.position_get().r_get());
            break;
        }
        case unit_action_id::attack: {
            leader_.attack(
                state->object_get(action.target_type_get(), action.target_player_get(), action.target_id_get()));
            break;
        }
        case unit_action_id::wait: {
            leader_.wait(action.time_get());
            break;
        }
        case unit_action_id::error: BOOST_LOG_TRIVIAL(error) << "unknown squad leader action";
    }

    switch (leader_.actual_action_get().action_type_get())
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
        object_abstract_class& enemy = state->object_get(leader_.actual_action_get().target_type_get(),
                                                         leader_.actual_action_get().target_player_get(),
                                                         leader_.actual_action_get().target_id_get());
                
        for (const auto unit : unit_list_)
        {
            if (unit->position_get().distance(enemy.position_get(), map) > attack_distance * 2)
                unit->follow(leader_);
            else
                unit->attack(enemy);
        }
        break;
    }
    case unit_action_id::wait: {
        for (const auto unit : unit_list_)
        {
            unit->wait(leader_.actual_action_get().time_get());
        }
        break;
    }
    case unit_action_id::error: BOOST_LOG_TRIVIAL(error) << "unknown squad leader action";
    }
    
}

unit_class& squad_class::leader_get() const
{
    return leader_;
}
