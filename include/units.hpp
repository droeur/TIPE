#pragma once


#include <vector>
#include <queue>

using frame = int;

#define ATTACK_COOLDOWN (frame)24
#define MOVE_COOLDOWN (frame)24

enum
{
    attack_distance = 4
};

enum class unit_type { normal };
enum class unit_action_id { error, move, attack, wait, pick, follow };

class unit_class;
class unit_action;
#include "object.hpp"
#include "position.hpp"
#include "food.hpp"

class unit_action
{
    unit_class* u_;
    unit_action_id action_type_;
    object_abstract_class* target_ = nullptr;
    position location_;
    time_t time_ = 0;

public:
    unit_action(unit_class* u, unit_action_id type, object_abstract_class* target);
    unit_action(unit_class* u, unit_action_id type, position target);
    unit_action(unit_class* u, unit_action_id type, time_t time);

    [[nodiscard]] unit_class* unit_get() const;
    [[nodiscard]] unit_action_id action_type_get() const;
    [[nodiscard]] object_abstract_class* target_unit_get() const;
    [[nodiscard]] time_t time_get() const;
    position& position_get();
};


class unit_class final : public object_abstract_class
{
    int t_a_ = 5, t_m_ = 5; // attack cooldown and move cooldown
    unit_action *actual_action_ = nullptr;
    std::queue<unit_action*> action_queue_;
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
    void actual_action_remove();
    [[nodiscard]] unit_action* actual_action_get() const;

    void move(int q, int r, bool queuing = false);
    void attack(object_abstract_class* b, bool queuing = false);
    void wait(time_t t, bool queuing = false);
    void follow(object_abstract_class* b, bool queuing = false);
    void pick(food_class* food, bool queuing = false);

    [[nodiscard]] bool can_move() const { return t_m_ == 0 && hit_point_ > 0; }
    [[nodiscard]] bool can_attack() const { return t_a_ == 0 && hit_point_ > 0; }

    void update_cooldown();
    void reinitialize_attack_cooldown(){t_a_ = ATTACK_COOLDOWN;}
    void reinitialize_move_cooldown(){t_m_ = MOVE_COOLDOWN;}

    void path_set(const std::vector<hex_tile*>& path);
    std::vector<hex_tile*>* path_get();

    [[nodiscard]] bool carry_food_get() const;
    void carry_food_set(bool c);

    void temporary_position_set(int q, int r);
    void temporary_position_apply();

    [[nodiscard]] int damage_per_frame_get() const;
    [[nodiscard]] int max_speed_get() const;
};
