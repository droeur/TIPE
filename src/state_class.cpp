#include <vector>
#include "game_class.hpp"
#include "state_class.hpp"
#include "units.hpp"
#include <SDL2/SDL.h>
#include <boost/log/trivial.hpp>

using namespace std;

void state_class::frame_increment()
{
    this->frame_++;
}

frame state_class::frame_get() const
{
    return frame_;
}

void state_class::unit_list_add()
{
    const vector<unit_class> u_list;
    this->list_of_u_list_.push_back(u_list);
}

std::vector<std::vector<unit_class>>& state_class::unit_list_get()
{
    return this->list_of_u_list_;
}

unit_class& state_class::unit_get(const player_id player, const object_id id)
{
    return this->list_of_u_list_[player][id];
}

void state_class::unit_append(const unit_class& u, const player_id player)
{
    return this->list_of_u_list_[player].push_back(u);
}

object_id state_class::unit_new(const int q, const int r, const player_id p_id, const int hp)
{
    const auto obj_id = static_cast<object_id>(list_of_u_list_[p_id].size());
    const unit_class u{q,r,p_id,hp, obj_id};
    unit_append(u, p_id);
    return obj_id;
}

void state_class::food_append(const food_class& f)
{
    food_list_.push_back(f);
}

std::vector<food_class>& state_class::food_list_get() 
{
    return food_list_;
}

object_id state_class::food_new(const int q, const int r)
{
    const auto obj_id = static_cast<object_id>(food_list_.size());
    const food_class f{q, r, obj_id};
    food_append(f);
    return obj_id;
}

void state_class::base_append(const base_class& b)
{
    base_list_.push_back(b);
}

std::vector<base_class>& state_class::base_list_get()
{
    return base_list_;
}

object_id state_class::base_new(const int q, const int r, const player_id p_id)
{

    const auto obj_id = static_cast<object_id>(base_list_.size());
    const base_class b{q, r, p_id, obj_id};
    base_append(b);
    return obj_id;
}

vector<unit_action> state_class::moves_generate(const player_id id, unit_class& unit)
{
    vector<unit_action> this_unit_action;

    // we switch between different possibles actions
    if (unit.can_attack())
    {
        const player_id enemy_player = game_class::enemy_player_get(id);
        for (auto& enemy_u : list_of_u_list_[enemy_player])
        {
            if (enemy_u.hp_get() > 0)
            {
                unit_action action{&unit, unit_action_id::attack, enemy_u};
                this_unit_action.push_back(action);
            }
        }
        for (auto& base : base_list_)
        {
            if (base.hp_get() > 0 && base.player_get() == enemy_player)
            {
                unit_action action{&unit, unit_action_id::attack, base};
                this_unit_action.push_back(action);
            }
        }
    }
    if (unit.can_move())
    {
        if (!unit.carry_food_get())
        {
            for (auto& food : food_list_)
            {
                unit_action action{&unit, unit_action_id::pick, food};
                this_unit_action.push_back(action);
            }
        }
        else
        {
            for (auto& base : base_list_)
            {
                if (base.hp_get() > 0 && base.player_get() == id)
                {
                    unit_action action{&unit, unit_action_id::move, base.position_get()};
                    this_unit_action.push_back(action);
                }
            }
        }
    }
    return this_unit_action;
}

vector<vector<unit_action>> state_class::moves_generate(const player_id id)
{
    vector<vector<unit_action>> possibles_actions;
    vector v{unit_action_id::error, unit_action_id::move, unit_action_id::attack, unit_action_id::wait};
    
    for (unit_class& u : list_of_u_list_[id])
    {
        possibles_actions.push_back(moves_generate(id, u));
    }
    return possibles_actions;
}

void state_class::fps_check_before()
{
    begin_frame_ = SDL_GetPerformanceCounter();
}

void state_class::fps_check_after(const bool fast)
{
    end_frame_before_ = SDL_GetPerformanceCounter();
    const double elapsed_ms_before =
        static_cast<double>(end_frame_before_ - begin_frame_) / static_cast<double>(SDL_GetPerformanceFrequency());
    if (!fast)
        SDL_Delay(static_cast<uint32_t>(floor(33.333 - elapsed_ms_before)));
    end_frame_after_ = SDL_GetPerformanceCounter();
    const double elapsed_ms_after =
        static_cast<double>(end_frame_after_ - begin_frame_) / static_cast<double>(SDL_GetPerformanceFrequency());
    fps_ = 1.0 / elapsed_ms_after;
}

void state_class::fps_set(const double f)
{
    fps_ = f;
}

double state_class::fps_get() const
{
    return fps_;
}


//TODO
int state_class::evaluate(const player_id player) const
{
    const player_id enemy = game_class::enemy_player_get(player);
    int score = 0;
    for (const auto& base : base_list_)
    {
        if (base.player_get() == player)
            score += base.hp_get();
        else
            score -= base.hp_get();
    }

    for (const auto& unit : list_of_u_list_[player])
        score += unit.hp_get();
    for (const auto& unit : list_of_u_list_[enemy])
        score -= unit.hp_get();
    return score;
}

std::vector<unit_action> state_class::action_vec_get(const player_id player)
{
    vector<unit_action> action_vec;
    for (auto& u : unit_list_get()[player])
    {
        action_vec.push_back(u.actual_action_get());
    }
    return action_vec;
}

void state_class::move_execute(unit_class* u, const position& p, const map_class* map)
{
    hex_tile* start = map->tile_get(u->q_get(), u->r_get());
    hex_tile* end = map->tile_get(p.q_get(), p.r_get());
    if (u->pathfinding_cooldown_get() == 0)
    {
        u->pathfinding_cooldown_reinitialize();
        u->path_get()->clear();
    }
    if (u->path_get()->empty())
    {
        const vector<hex_tile*> path = map->path_a_star(start, end);
        u->path_set(path);
    }
    if (!u->path_get()->empty())
    {
        u->temporary_position_set((*u->path_get())[0]->q(), (*u->path_get())[0]->r());
        u->path_get()->erase(u->path_get()->begin());
    }
}

void state_class::action_execute(unit_action* action, unit_class* unit, const map_class *map)
{
    switch (action->action_type_get())
    {
    case unit_action_id::attack: {
        object_abstract_class& enemy_obj = object_get(action->target_type_get(), action->target_player_get(), action->target_id_get());
        if (const int distance = unit->position_get().distance(enemy_obj.position_get(), map);
            attack_distance > distance)
        {
            attack_execute(*unit, enemy_obj);
            unit->actual_action_remove();
            unit->path_get()->clear();
        }
        else
        {
            const position& p = enemy_obj.position_get();
            move_execute(unit, p, map);
        }
    }
    break;
    case unit_action_id::pick: {
        object_abstract_class& food = object_get(action->target_type_get(), action->target_player_get(), action->target_id_get());
        if (const double distance = unit->position_get().distance(food.position_get(), map); distance < 1)
        {
            unit->actual_action_remove();
            unit->path_get()->clear();
            if (food.id_get() == -1 && !unit->carry_food_get())
            {
                // to avoid duplication
                unit->carry_food_set(true);
                food.id_set(0);
                vector<food_class*>::size_type i = 0;
                for (auto &f : food_list_)
                {
                    if (f.position_get() == food.position_get() && i < food_list_.size())
                    {
                        food_list_.erase(food_list_.begin() + static_cast<int>(i));
                    }
                    i++;
                }
            }
        }
        else
        {
            const position& p = food.position_get();
            move_execute(unit, p, map);
        }
    }
    break;
    case unit_action_id::move: {
        const position& p = action->position_get();
        if (const double distance = unit->position_get().distance(p, map); distance < 1)
        {
            unit->actual_action_remove();
            unit->path_get()->clear();
        }
        else
        {
            move_execute(unit, p, map);
        }
    }
    break;
    case unit_action_id::error:
    case unit_action_id::wait:
    default: break;
    }
}

void state_class::moves_make(const map_class *map)
{
    for (int player = 0; player < number_of_players; player++)
    {
        int u_ind = 0;
        for (auto &unit : list_of_u_list_[player])
        {
            if (unit_action action = unit.actual_action_get(); action.action_type_get() != unit_action_id::nothing){
                action_execute(&action, &unit, map);
            }
            u_ind++;
        }
    }
    for (int player = 0; player < number_of_players; player++)
    {
        vector<unit_class>::size_type size = list_of_u_list_[player].size();
        for (vector<unit_class>::size_type index = 0; index < size; ++index)
        {
            unit_class& u = list_of_u_list_[player][index];
            u.update_cooldown();
            if (u.temporary_hp_get() == u.hp_get())
                u.temporary_position_apply();
            u.temporary_hp_apply();
            if (u.carry_food_get())
            {
                for (auto base : base_list_)
                {
                    if (base.player_get() == player && base.position_get().distance(u.position_get(), map) < 1)
                    {
                        u.carry_food_set(false);
                        unit_new(base.q_get(), base.r_get(), u.player_get(), 10);
                        ++size;
                    }
                }
            }
        }
    }
    for (auto base : base_list_)
    {
        base.temporary_hp_apply();
    }
}

object_abstract_class& state_class::object_get(const object_type type, const player_id player, const object_id id)
{
    switch (type)
    {
    case object_type::base: 
        return base_list_get()[id];
    case object_type::unit: 
        return unit_get(player, id);
    case object_type::food: 
        return food_list_get()[id];
    }
    return food_list_get()[id];
}

void state_class::attack_execute(unit_class& u, object_abstract_class& enemy_obj)
{
    enemy_obj.temporary_hp_remove(1);
    if (enemy_obj.hp_get() - 1 < 0)
    {
        enemy_obj.temporary_hp_set(0);
        if (enemy_obj.object_type_get() == object_type::unit)
        {
            if (const auto enemy_u = dynamic_cast<unit_class&>(enemy_obj); enemy_u.carry_food_get())
            {
                food_new(enemy_u.q_get(), enemy_u.r_get());
            }
        }
    }
    BOOST_LOG_TRIVIAL(debug) << "unit " << &u << " ,player " << u.player_get() << " attacked unit " << &enemy_obj << " ,player " << enemy_obj.player_get()
                << " hp = " << enemy_obj.hp_get();
    u.reinitialize_attack_cooldown();
}

