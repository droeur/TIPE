#pragma once


#include <vector>

using frame = int;

#define ATTACK_COOLDOWN (frame)24

enum
{
    attack_distance = 4
};

#define MOVE_COOLDOWN (frame)24

#define SPEED_FACTOR 0.5

enum class u_type { normal };

enum class u_action_id { error, move, attack, wait, pick };

class unit_class;
class unit_action;
#include "object.hpp"
#include "position.hpp"
#include "state_class.hpp"

class unit_action
{
public:
    unit_action(unit_class* u, const u_action_id type, object_abstract_class* target, const frame remaining_frames)
        : u_(u),
          action_type_(type),
          target_(target),
          remaining_frames_(remaining_frames)
    {
    }

    unit_action(unit_class* u, const u_action_id type, const position target, const frame remaining_frames)
        : u_(u),
          action_type_(type),
          remaining_frames_(remaining_frames)
    {
        p_ = target;
    }

    [[nodiscard]]
    unit_class* unit_get() const
    {
        return u_;
    }

    [[nodiscard]]
    u_action_id action_type_get() const
    {
        return action_type_;
    }

    [[nodiscard]]
    object_abstract_class* target_unit_get() const
    {
        return target_;
    }

    position& position_get()
    {
        return p_;
    }

protected:
    unit_class* u_;
    u_action_id action_type_;
    object_abstract_class* target_ = nullptr;
    frame remaining_frames_;
    position p_;
};


class unit_class final : public object_abstract_class
{
public:
    unit_class(const int q, const int r, const player_id id, const int hp)
        : object_abstract_class(q, r, hp, id, object_type::undefined)
    {
        t_a_ = 5;
        t_m_ = 5;
        actual_action_ = nullptr;
    }

    ~unit_class() override = default;

    void actual_action_set(unit_action* action)
    {
        actual_action_ = action;
    }

    [[nodiscard]] unit_action* actual_action_get() const
    {
        return actual_action_;
    }

    //actions
    void move(double x, double y);
    void attack(object_abstract_class* b, state_class* s);
    void wait(time_t t);

    [[nodiscard]] bool can_move() const { return t_m_ == 0 && hp_ > 0; }
    [[nodiscard]] bool can_attack() const { return t_a_ == 0 && hp_ > 0; }

    void update_cooldown()
    {
        if (t_m_ > 0)
            t_m_--;
        else
            t_m_ = 0;
        if (t_a_ > 0)
            t_a_--;
        else
            t_a_ = 0;
    }

    void path_set(const std::vector<hex_tile*>& path)
    {
        path_ = path;
    }

    std::vector<hex_tile*>* path_get()
    {
        return &path_;
    }

    [[nodiscard]] bool carry_food_get() const
    {
        return carry_food_;
    }

    void carry_food_set(const bool c)
    {
        carry_food_ = c;
    }

    void temporary_position_set(const int q, const int r)
    {
        this->temporary_p_.q_set(q);
        this->temporary_p_.r_set(r);
    }
    void temporary_position_apply()
    {
        if (temporary_p_.q_get() != -1 && temporary_p_.r_get() != -1)
            p_ = temporary_p_;
    }

private:
    int t_a_, t_m_; // attack cooldown and move cooldown
    unit_action* actual_action_;
    std::vector<hex_tile*> path_;
    bool carry_food_ = false;
    position temporary_p_{-1, -1};
};
