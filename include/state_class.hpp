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
    void attack_execute(unit_class& u, object_abstract_class& enemy_obj);
    void action_execute(unit_action* action, unit_class* unit, const map_class* map);
    
    frame frame_ = 0;
    std::vector<std::vector<unit_class>> list_of_u_list_;
    double fps_ = 0;
    std::vector<food_class> food_list_;
    std::vector<base_class> base_list_;
    uint64_t begin_frame_ = 1;
    uint64_t end_frame_before_ = 1;
    uint64_t end_frame_after_ = 1;

    void unit_append(const unit_class& u, player_id player);
    void base_append(const base_class& b);
    void food_append(const food_class& f);

    options_class* options_{};

public:
    state_class() = default;

    void frame_increment();
    [[nodiscard]] frame frame_get() const;

    [[nodiscard]] std::vector<std::vector<unit_action>> moves_generate(player_id id);
    [[nodiscard]] std::vector<unit_action> moves_generate(player_id id, unit_class& unit);
    void moves_make(const map_class* map);

    object_abstract_class& object_get(const object_type type, const player_id player, const object_id id);

    void unit_list_add();
    std::vector<std::vector<unit_class>>& unit_list_get();
    unit_class& unit_get(const player_id player, const object_id id);
    object_id unit_new(int q, int r, player_id p_id, int hp);

    std::vector<food_class>& food_list_get();
    object_id food_new(int q, int r);

    std::vector<base_class>& base_list_get();
    object_id base_new(const int q, const int r, const player_id p_id);

    void fps_check_before();
    void fps_check_after(bool fast);

    void fps_set(double f);
    [[nodiscard]] double fps_get() const;

    [[nodiscard]] int evaluate(player_id player) const;
    std::vector<unit_action> action_vec_get(player_id player);

    [[nodiscard]] options_class& options_get() const { return *options_; }
    void options_set(options_class& options) { options_ = &options; }
};
