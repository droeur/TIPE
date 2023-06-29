#pragma once

#include <vector>

class state;

#include "units.hpp"
#include "hex_map.hpp"
#include "food.hpp"
#include "base.hpp"
#include "game.hpp"

using frame = int;

class state
{
public:
    state(map_class* map, game* g)
    {
        frame_ = 0;
        this->map_ = map;
        this->game_ = g;
        const vector<vector<unit_action>> possible_action_list1;
        possibles_actions_[0] = possible_action_list1;
        const vector<vector<unit_action>> possible_action_list2;
        possibles_actions_[1] = possible_action_list2;
    }

    void frame_increment();
    frame frame_get() const;

    void moves_generate();
    void moves_make();

    vector<vector<unit_action>>* possibles_actions_get()
    {
        return possibles_actions_;
    }

    void chosen_actions_set(const vector<unit_action>& chosen_list, const player_id id)
    {
        chosen_actions_[id].clear();
        chosen_actions_[id] = chosen_list;
    }

    vector<unit_action> chosen_actions_get(const player_id id)
    {
        return chosen_actions_[id];
    }

    void unit_list_add(const vector<unit>& u_list);
    vector<vector<unit>> unit_list_get();
    void unit_append(const unit& u, player_id player);

    void food_append(const food_class& f)
    {
        food_list_.push_back(f);
    }

    vector<food_class>* food_list_get()
    {
        return &food_list_;
    }

    void base_append(const base_class& f)
    {
        base_list_.push_back(f);
    }

    vector<base_class>* base_list_get()
    {
        return &base_list_;
    }

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
    vector<vector<unit>> u_list_;
    vector<vector<unit_action>> possibles_actions_[NUMBER_OF_PLAYERS]; //vector of possible actions
    vector<unit_action> chosen_actions_[NUMBER_OF_PLAYERS];            //vector of possible actions
    map_class* map_;
    game* game_;
    double fps_ = 0;
    vector<food_class> food_list_;
    vector<base_class> base_list_;
};
