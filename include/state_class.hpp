#pragma once

#include <vector>

class state_class;

#include "game_class.hpp"
#include "hex_map.hpp"
#include "food.hpp"
#include "base.hpp"

using frame = int;

class state_class
{
public:
    state_class(map_class* map, game_class* g)
    {
        frame_ = 0;
        this->map_ = map;
        this->game_ = g;
        const std::vector<std::vector<unit_action>> possible_action_list1;
        possibles_actions_[0] = possible_action_list1;
        const std::vector<std::vector<unit_action>> possible_action_list2;
        possibles_actions_[1] = possible_action_list2;
    }

    void frame_increment();
    [[nodiscard]] frame frame_get() const;

    void moves_generate();
    void moves_make();

    std::vector<std::vector<unit_action>>* possibles_actions_get()
    {
        return possibles_actions_;
    }

    void chosen_actions_set(const std::vector<unit_action>& chosen_list, const player_id id)
    {
        chosen_actions_[id].clear();
        chosen_actions_[id] = chosen_list;
    }

    std::vector<unit_action> chosen_actions_get(const player_id id)
    {
        return chosen_actions_[id];
    }

    void unit_list_add(const std::vector<unit>& u_list);
    std::vector<std::vector<unit>> unit_list_get();
    void unit_append(const unit& u, player_id player);

    void food_append(const food_class& f)
    {
        food_list_.push_back(f);
    }

    std::vector<food_class>* food_list_get()
    {
        return &food_list_;
    }

    void base_append(const base_class& f)
    {
        base_list_.push_back(f);
    }

    std::vector<base_class>* base_list_get()
    {
        return &base_list_;
    }

    void fps_check_before();
    void fps_check_after();

    void fps_set(const double f)
    {
        fps_ = f;
    }

    [[nodiscard]] double fps_get() const
    {
        return fps_;
    }

    [[nodiscard]] map_class* map_get() const
    {
        return map_;
    }

private:
    void move_to_place(unit* u, position& p) const;
    frame frame_;
    std::vector<std::vector<unit>> u_list_;
    std::vector<std::vector<unit_action>> possibles_actions_[number_of_players]; // vector of possible actions
    std::vector<unit_action> chosen_actions_[number_of_players];                 // vector of possible actions
    map_class* map_;
    game_class* game_;
    double fps_ = 0;
    std::vector<food_class> food_list_;
    std::vector<base_class> base_list_;
    uint64_t begin_frame_;
    uint64_t end_frame_before_;
    uint64_t end_frame_after_;
};
