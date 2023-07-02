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

void state_class::unit_list_add(const vector<unit>& u_list)
{
    this->u_list_.push_back(u_list);
}

vector<vector<unit>> state_class::unit_list_get()
{
    return this->u_list_;
}

void state_class::unit_append(const unit& u, const player_id player)
{
    return this->u_list_[player].push_back(u);
}

void state_class::moves_generate()
{
    vector v{u_action_id::error, u_action_id::move, u_action_id::attack, u_action_id::wait};

    for (int player = 0; player < number_of_players; player++)
    {
        possibles_actions_[player].clear();
        for (unit& u : u_list_[player])
        {
            vector<unit_action> this_unit_action;

            // we switch between different possibles actions
            if (u.can_attack())
            {
                const player_id enemy_player = game_->enemy_player_get(player);
                for (unit& enemy_u : u_list_[enemy_player])
                {
                    if (enemy_u.hp_get() > 0)
                    {
                        unit_action action{&u, u_action_id::attack, &enemy_u, ATTACK_COOLDOWN};
                        this_unit_action.push_back(action);
                    }
                }
                for (base_class& base : base_list_)
                {
                    if (base.hp_get() > 0 && base.player_get() == enemy_player)
                    {
                        unit_action action{&u, u_action_id::attack, &base, ATTACK_COOLDOWN};
                        this_unit_action.push_back(action);
                    }
                }
            }
            if (u.can_move())
            {
                if (!u.carry_food_get())
                {
                    for (food_class& food : food_list_)
                    {
                        unit_action action{&u, u_action_id::pick, &food, MOVE_COOLDOWN};
                        this_unit_action.push_back(action);
                    }
                }
                else
                {
                    for (base_class& base : base_list_)
                    {
                        if (base.hp_get() > 0 && base.player_get() == player)
                        {
                            unit_action action{&u, u_action_id::move, base.position_get(), MOVE_COOLDOWN};
                            this_unit_action.push_back(action);
                        }
                    }
                }
            }
            possibles_actions_[player].push_back(this_unit_action);
        }

    }
}

void state_class::fps_check_before()
{
    begin_frame_ = SDL_GetPerformanceCounter();
}

void state_class::fps_check_after()
{
    end_frame_before_ = SDL_GetPerformanceCounter();
    const double elapsed_ms_before =
        static_cast<double>(end_frame_before_ - begin_frame_) / static_cast<double>(SDL_GetPerformanceFrequency());
    SDL_Delay(static_cast<uint32_t>(floor(33.3333 - elapsed_ms_before)));
    end_frame_after_ = SDL_GetPerformanceCounter();
    const double elapsed_ms_after =
        static_cast<double>(end_frame_after_ - begin_frame_) / static_cast<double>(SDL_GetPerformanceFrequency());
    fps_ = 1.0 / elapsed_ms_after;
}

void state_class::move_to_place(unit* u, position& p) const
{
    hex_tile* start = map_->tile_get(u->q_get(), u->r_get());
    hex_tile* end = map_->tile_get(p.getQ(), p.getR());
    if (u->path_get()->empty())
    {
        const vector<hex_tile*> path = map_->path_a_star(start, end);
        u->path_set(path);
    }
    else
    {
        u->position_set((*u->path_get())[0]->q(), (*u->path_get())[0]->r());
        u->path_get()->erase(u->path_get()->begin());
    }
}

/**
 * @brief Pour update tout -> fait les actions et réduits les cooldowns
 * 
 */
void state_class::moves_make()
{
    for (int player = 0; player < number_of_players; player++)
    {
        if (!chosen_actions_[player].empty())
        {
            for (auto action = chosen_actions_[player].begin(); action != chosen_actions_[player].end();)
            {
                unit* u = action->unit_get();
                switch (action->action_type_get())
                {
                case u_action_id::attack: {
                    object_abstract_class* enemy_u = action->target_unit_get();
                    if (const double distance = u->position_get().distance(enemy_u->position_get(), map_);
                        ATTACK_DISTANCE > distance)
                    {
                        u->attack(enemy_u, *this);
                        action = chosen_actions_[player].erase(action);
                        u->path_get()->clear();
                    }
                    else
                    {
                        position p = enemy_u->position_get();
                        move_to_place(u, p);
                        ++action;
                    }
                }
                break;
                case u_action_id::pick: {
                    object_abstract_class* food = action->target_unit_get();
                    if (const double distance = u->position_get().distance(food->position_get(), map_); distance < 1)
                    {
                        action = chosen_actions_[player].erase(action);
                        u->path_get()->clear();
                        cout << food->id_get() << u->carry_food_get() << endl;
                        if (food->id_get() == -1 && !u->carry_food_get())
                        {
                            // to avoid duplication
                            u->carry_food_set(true);
                            food->id_set(0);
                            int i = 0;
                            for (food_class f : food_list_)
                            {
                                if (f.position_get() == food->position_get() && i < food_list_.size())
                                {
                                    food_list_.erase(food_list_.begin() + i);
                                }
                                i++;
                            }
                        }
                    }
                    else
                    {
                        position p = food->position_get();
                        move_to_place(u, p);
                        ++action;
                    }

                }
                break;
                case u_action_id::move: {
                    position& p = action->position_get();
                    if (const double distance = u->position_get().distance(p, map_); distance < 1)
                    {
                        action = chosen_actions_[player].erase(action);
                        u->path_get()->clear();
                    }
                    else
                    {
                        move_to_place(u, p);
                        ++action;
                    }
                }
                break;
                case u_action_id::error:
                case u_action_id::wait: ++action;
                    break;
                }
            }
        }
        vector<unit>::size_type size = u_list_[player].size();
        for (vector<unit>::size_type index = 0; index < size; ++index)
        {
            unit &u = u_list_[player][index];
            u.update_cooldown();
            if (u.carry_food_get())
            {
                for (base_class& base : base_list_)
                {
                    if (base.player_get() == player && base.position_get().distance(u.position_get(), map_) < 1)
                    {
                        u.carry_food_set(false);
                        const int q = base.q_get();
                        const int r = base.r_get();
                        unit u_new{q, r, u.player_get(), 10};
                        u_list_[player].push_back(u_new);
                        ++size;
                    }
                }
            }
        }
    }
}
