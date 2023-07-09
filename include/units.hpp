#pragma once


#include <vector>

using frame = int;

#define ATTACK_COOLDOWN (frame)24
#define MOVE_COOLDOWN (frame)24

enum
{
    attack_distance = 4
};

enum class unit_type { normal };
enum class unit_action_id { error, move, attack, wait, pick };

class unit_class;
class unit_action;
#include "object.hpp"
#include "position.hpp"

class unit_action
{
    unit_class* u_;
    unit_action_id action_type_;
    object_abstract_class* target_ = nullptr;
    position location_;

public:
    unit_action(unit_class* u, unit_action_id type, object_abstract_class* target);
    unit_action(unit_class* u, unit_action_id type, position target);

    [[nodiscard]] unit_class* unit_get() const;
    [[nodiscard]] unit_action_id action_type_get() const;
    [[nodiscard]] object_abstract_class* target_unit_get() const;
    position& position_get();
};


class unit_class final : public object_abstract_class
{
    int t_a_ = 5, t_m_ = 5; // attack cooldown and move cooldown
    unit_action *actual_action_ = nullptr;
    std::vector<hex_tile*> path_;
    bool carry_food_ = false;
    position temporary_p_{-1, -1};
    int damage_per_frame_ = 1;
    int max_speed_ = 1;

public:
    unit_class(const int q, const int r, const player_id id, const int hp)
        : object_abstract_class(q, r, hp, id, object_type::undefined) {}

    ~unit_class() override = default;


    //actions
    void actual_action_set(unit_action* action);
    void actual_action_remove();
    [[nodiscard]] unit_action* actual_action_get() const;

    void move(double x, double y);
    void attack(object_abstract_class* b);
    void wait(time_t t);

    [[nodiscard]] bool can_move() const { return t_m_ == 0 && hit_point_ > 0; }
    [[nodiscard]] bool can_attack() const { return t_a_ == 0 && hit_point_ > 0; }

    void update_cooldown();
    void reinitialize_attack_cooldown(){t_a_ = ATTACK_COOLDOWN;}

    void path_set(const std::vector<hex_tile*>& path);
    std::vector<hex_tile*>* path_get();

    [[nodiscard]] bool carry_food_get() const;
    void carry_food_set(bool c);

    void temporary_position_set(int q, int r);
    void temporary_position_apply();

    [[nodiscard]] int damage_per_frame_get() const;
    [[nodiscard]] int max_speed_get() const;
};
