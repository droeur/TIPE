#pragma once


#include <vector>
#include <queue>

using frame = int;

#define ATTACK_COOLDOWN (frame)12
#define MOVE_COOLDOWN (frame)12
#define PATHFINDING_COOLDOWN (frame)12

enum
{
    attack_distance = 4
};

enum class unit_type { normal };
enum class unit_action_id { error, move, attack, wait, pick, follow, nothing };

class unit_class;
class unit_action;
#include "object.hpp"
#include "position.hpp"
#include "food.hpp"

class unit_action
{
    object_id u_id_;
    player_id player_id_;
    unit_action_id action_type_;
    object_id target_id_ = -1;
    object_type target_type_;
    player_id target_player_ = -1;
    position location_;
    time_t time_ = 0;

public:
    unit_action(object_id u_id, player_id player_id, unit_action_id type, const object_abstract_class& target);
    unit_action(object_id u_id, player_id player_id, const unit_action_id type, position target);
    unit_action(object_id u_id, player_id player_id, const unit_action_id type, time_t time);
    unit_action(object_id u_id, player_id player_id, const unit_action_id type);

    [[nodiscard]] int unit_id_get() const;
    [[nodiscard]] player_id unit_player_id_get() const;
    [[nodiscard]] unit_action_id action_type_get() const;
    [[nodiscard]] object_id target_id_get() const;
    [[nodiscard]] object_type target_type_get() const;
    [[nodiscard]] player_id target_player_get() const;
    [[nodiscard]] time_t time_get() const;
    position& position_get();
};


class unit_class final : public object_abstract_class
{
    int t_a_ = 5, t_m_ = 5; // attack cooldown and move cooldown
    int pathfinding_cooldown_ = 0;
    std::queue<unit_action> action_queue_;
    std::vector<hex_tile*> path_;
    bool carry_food_ = false;
    position temporary_p_{-1, -1};
    int damage_per_frame_ = 1;
    int max_speed_ = 1;

public:
    unit_class(const int q, const int r, const player_id p_id, const int hp, const object_id obj_id)
        : object_abstract_class(q, r, hp, p_id, object_type::unit, obj_id)
    {
    }

    unit_class(const unit_class& u)
        : object_abstract_class(u.q_get(), u.r_get(), u.hit_point_, u.player_, object_type::unit, u.id_)
    {
        *this = u;
        const std::queue<unit_action> queue;
        action_queue_ = queue;
    }

    ~unit_class() override = default;

    // actions
    void actual_action_remove();
    [[nodiscard]] unit_action actual_action_get() const;

    void move(int q, int r, bool queuing = false);
    void attack(object_abstract_class& b, bool queuing = false);
    void wait(time_t t, bool queuing = false);
    void follow(object_abstract_class& b, bool queuing = false);
    void pick(const food_class& food, bool queuing = false);

    [[nodiscard]] bool can_move() const { return t_m_ == 0 && hit_point_ > 0; }
    [[nodiscard]] bool can_attack() const { return t_a_ == 0 && hit_point_ > 0; }
    [[nodiscard]] bool can_wait() const { return hit_point_ > 0; }

    void update_cooldown();
    [[nodiscard]] int pathfinding_cooldown_get() const { return pathfinding_cooldown_; }
    void pathfinding_cooldown_reinitialize() { pathfinding_cooldown_ = PATHFINDING_COOLDOWN; }
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
