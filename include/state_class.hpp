#pragma once

#include <vector>


class state_class;
#include "units.hpp"
#include "food.hpp"
#include "base.hpp"
#include "game_class.hpp"

using frame = int;

class state_class
{
    static void move_execute(unit_class* u, const position& p, const map_class* map);
    void attack_execute(unit_class* u, object_abstract_class* enemy_obj);
    void action_execute(unit_action* action, unit_class* unit, const map_class* map);
    frame frame_ = 0;
    std::vector<std::vector<unit_class*>> u_list_;
    double fps_ = 0;
    std::vector<food_class*> food_list_;
    std::vector<base_class*> base_list_;
    uint64_t begin_frame_ = 1;
    uint64_t end_frame_before_ = 1;
    uint64_t end_frame_after_ = 1;

public:
    state_class() = default;

    ~state_class();

    void frame_increment();
    [[nodiscard]] frame frame_get() const;

    [[nodiscard]] std::vector<std::vector<unit_action>> moves_generate(player_id id) const;
    [[nodiscard]] std::vector<unit_action> moves_generate(player_id id, unit_class* unit) const;
    void moves_make(const map_class* map);

    void unit_list_add(const std::vector<unit_class*>& u_list);
    std::vector<std::vector<unit_class*>> unit_list_get();
    void unit_append(unit_class* u, player_id player);

    void food_append(food_class* f);
    std::vector<food_class*>* food_list_get();

    void base_append(base_class* b);
    std::vector<base_class*>* base_list_get();

    void fps_check_before();
    void fps_check_after(bool fast);

    void fps_set(double f);
    [[nodiscard]] double fps_get() const;

    int evaluate(player_id player);
};
