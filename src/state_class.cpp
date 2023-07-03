#include <vector>
#include "game_class.hpp"
#include "state_class.hpp"
#include "units.hpp"
#include <SDL3/SDL.h>

using namespace std;

void state_class::frame_increment()
{
    this->frame_++;
}

frame state_class::frame_get() const
{
    return frame_;
}

void state_class::unit_list_add(const vector<unit_class*>& u_list)
{
    this->u_list_.push_back(u_list);
}

std::vector<std::vector<unit_class*>> state_class::unit_list_get()
{
    return this->u_list_;
}

void state_class::unit_append(unit_class* u, const player_id player)
{
    return this->u_list_[player].push_back(u);
}

vector<vector<unit_action>> state_class::moves_generate(const player_id id) const
{
    vector<vector<unit_action>> possibles_actions;
    vector v{u_action_id::error, u_action_id::move, u_action_id::attack, u_action_id::wait};
    
    possibles_actions.clear();
    for (const auto u : u_list_[id])
    {
        vector<unit_action> this_unit_action;

        // we switch between different possibles actions
        if (u->can_attack())
        {
            const player_id enemy_player = game_class::enemy_player_get(id);
            for (const auto enemy_u : u_list_[enemy_player])
            {
                if (enemy_u->hp_get() > 0)
                {
                    unit_action action{u, u_action_id::attack, enemy_u, ATTACK_COOLDOWN};
                    this_unit_action.push_back(action);
                }
            }
            for (const auto base : base_list_)
            {
                if (base->hp_get() > 0 && base->player_get() == enemy_player)
                {
                    unit_action action{u, u_action_id::attack, base, ATTACK_COOLDOWN};
                    this_unit_action.push_back(action);
                }
            }
        }
        if (u->can_move())
        {
            if (!u->carry_food_get())
            {
                for (const auto food : food_list_)
                {
                    unit_action action{u, u_action_id::pick, food, MOVE_COOLDOWN};
                    this_unit_action.push_back(action);
                }
            }
            else
            {
                for (const auto base : base_list_)
                {
                    if (base->hp_get() > 0 && base->player_get() == id)
                    {
                        unit_action action{u, u_action_id::move, base->position_get(), MOVE_COOLDOWN};
                        this_unit_action.push_back(action);
                    }
                }
            }
        }
        possibles_actions.push_back(this_unit_action);
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
        SDL_Delay(static_cast<uint32_t>(floor(33.3333 - elapsed_ms_before)));
    end_frame_after_ = SDL_GetPerformanceCounter();
    const double elapsed_ms_after =
        static_cast<double>(end_frame_after_ - begin_frame_) / static_cast<double>(SDL_GetPerformanceFrequency());
    fps_ = 1.0 / elapsed_ms_after;
}

void state_class::move_to_place(unit_class* u, const position& p, map_class* map)
{
    hex_tile* start = map->tile_get(u->q_get(), u->r_get());
    hex_tile* end = map->tile_get(p.q_get(), p.r_get());
    if (u->path_get()->empty())
    {
        const vector<hex_tile*> path = map->path_a_star(start, end);
        u->path_set(path);
    }
    else
    {
        u->temporary_position_set((*u->path_get())[0]->q(), (*u->path_get())[0]->r());
        u->path_get()->erase(u->path_get()->begin());
    }
}

void state_class::moves_make(map_class *map)
{
    for (int player = 0; player < number_of_players; player++)
    {
        if (!chosen_actions_[player].empty())
        {
            for (auto action = chosen_actions_[player].begin(); action != chosen_actions_[player].end();)
            {
                unit_class* u = action->unit_get();
                switch (action->action_type_get())
                {
                case u_action_id::attack: {
                    object_abstract_class* enemy_u = action->target_unit_get();
                    if (const int distance = u->position_get().distance(enemy_u->position_get(), map);
                        attack_distance > distance)
                    {
                        u->attack(enemy_u, this);
                        action = chosen_actions_[player].erase(action);
                        u->path_get()->clear();
                    }
                    else
                    {
                        position p = enemy_u->position_get();
                        move_to_place(u, p, map);
                        ++action;
                    }
                }
                break;
                case u_action_id::pick: {
                    object_abstract_class* food = action->target_unit_get();
                    if (const double distance = u->position_get().distance(food->position_get(), map); distance < 1)
                    {
                        action = chosen_actions_[player].erase(action);
                        u->path_get()->clear();
                        cout << food->id_get() << u->carry_food_get() << endl;
                        if (food->id_get() == -1 && !u->carry_food_get())
                        {
                            // to avoid duplication
                            u->carry_food_set(true);
                            food->id_set(0);
                            vector<food_class*>::size_type i = 0;
                            for (const auto f : food_list_)
                            {
                                if (f->position_get() == food->position_get() && i < food_list_.size())
                                {
                                    food_list_.erase(food_list_.begin() + static_cast<int>(i));
                                }
                                i++;
                            }
                        }
                    }
                    else
                    {
                        position p = food->position_get();
                        move_to_place(u, p, map);
                        ++action;
                    }

                }
                break;
                case u_action_id::move: {
                    position& p = action->position_get();
                    if (const double distance = u->position_get().distance(p, map); distance < 1)
                    {
                        action = chosen_actions_[player].erase(action);
                        u->path_get()->clear();
                    }
                    else
                    {
                        move_to_place(u, p, map);
                        ++action;
                    }
                }
                break;
                case u_action_id::error:
                case u_action_id::wait:
                    ++action;
                    break;
                }
            }
        }
    }
    for (int player = 0; player < number_of_players; player++)
    {
        vector<unit_class*>::size_type size = u_list_[player].size();
        for (vector<unit_class*>::size_type index = 0; index < size; ++index)
        {
            unit_class* u = u_list_[player][index];
            u->update_cooldown();
            u->temporary_position_apply();
            if (u->carry_food_get())
            {
                for (const auto base : base_list_)
                {
                    if (base->player_get() == player && base->position_get().distance(u->position_get(), map) < 1)
                    {
                        u->carry_food_set(false);
                        const int q = base->q_get();
                        const int r = base->r_get();
                        auto u_new = new unit_class{q, r, u->player_get(), 10};
                        u_list_[player].push_back(u_new);
                        ++size;
                    }
                }
            }
        }
    }
}
