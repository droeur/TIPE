#pragma once

#include <vector>


class state_class;
#include "food.hpp"
#include "base.hpp"
#include "game_class.hpp"
#include "units.hpp"

using frame = int;

class state_class
{
public:
    state_class()
    {
        frame_ = 0;
    }

    ~state_class()
    {
        for (const auto f:food_list_)
        {
            delete f;
        }
        for (const auto b:base_list_)
        {
            delete b;
        }
        for (const auto &u_l:u_list_)
        {
            for (const auto u : u_l)
            {
                delete u;
            }
        }
    }

    void frame_increment();
    [[nodiscard]] frame frame_get() const;

    [[nodiscard]] std::vector<std::vector<unit_action>> moves_generate(player_id id) const;
    void moves_make(map_class* map);

    void chosen_actions_set(const std::vector<unit_action>& chosen_list, const player_id id)
    {
        chosen_actions_[id].clear();
        chosen_actions_[id] = chosen_list;
    }

    std::vector<unit_action> chosen_actions_get(const player_id id)
    {
        return chosen_actions_[id];
    }

    void unit_list_add(const std::vector<unit_class*>& u_list);
    std::vector<std::vector<unit_class*>> unit_list_get();
    void unit_append(unit_class* u, player_id player);

    void food_append(food_class* f)
    {
        food_list_.push_back(f);
    }

    std::vector<food_class*>* food_list_get()
    {
        return &food_list_;
    }

    void base_append(base_class* b)
    {
        base_list_.push_back(b);
    }

    std::vector<base_class*>* base_list_get()
    {
        return &base_list_;
    }

    void fps_check_before();
    void fps_check_after(bool fast);

    void fps_set(const double f)
    {
        fps_ = f;
    }

    [[nodiscard]] double fps_get() const
    {
        return fps_;
    }

private:
    static void move_to_place(unit_class* u, const position& p, map_class *map);
    frame frame_ = 0;
    std::vector<std::vector<unit_class*>> u_list_;
    std::vector<unit_action> chosen_actions_[number_of_players];                 // vector of possible actions
    double fps_ = 0;
    std::vector<food_class*> food_list_;
    std::vector<base_class*> base_list_;
    uint64_t begin_frame_ = 1;
    uint64_t end_frame_before_ = 1;
    uint64_t end_frame_after_ = 1;
};
