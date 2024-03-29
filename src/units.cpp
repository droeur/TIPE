#include "units.hpp"
#include "food.hpp"

using namespace std;

unit_action::unit_action(const int u_id, const player_id player_id, const unit_action_id type,
                         const object_abstract_class& target)
    : u_id_(u_id)
    , player_id_(player_id)
    , action_type_(type)
    , target_id_(target.id_get())
    , target_type_(target.object_type_get())
    , target_player_(target.player_get())
{
}

unit_action::unit_action(const int u_id, const player_id player_id, const unit_action_id type, const position target)
    : u_id_(u_id)
    , player_id_(player_id)
    , action_type_(type)
    , location_(target)
{
}

unit_action::unit_action(const int u_id, const player_id player_id, const unit_action_id type, const time_t time)
    : u_id_(u_id)
    , player_id_(player_id)
    , action_type_(type)
    , time_(time)
{
}

unit_action::unit_action(const int u_id, const player_id player_id, const unit_action_id type)
    : u_id_(u_id)
    , player_id_(player_id)
    , action_type_(type)
{
}

int unit_action::unit_id_get() const
{
    return u_id_;
}

player_id unit_action::unit_player_id_get() const
{
    return player_id_;
}

unit_action_id unit_action::action_type_get() const
{
    return action_type_;
}

object_id unit_action::target_id_get() const
{
    return target_id_;
}

object_type unit_action::target_type_get() const
{
    return target_type_;
}

player_id unit_action::target_player_get() const
{
    return target_player_;
}

time_t unit_action::time_get() const
{
    return time_;
}

position& unit_action::position_get()
{
    return location_;
}

void unit_class::actual_action_remove()
{
    action_queue_.pop();
}

/**
 * \brief Returns the actual action
 * \return Actual action or nullptr if empty
 */
unit_action unit_class::actual_action_get() const
{
    if (action_queue_.empty())
        return unit_action{this->id_, this->player_, unit_action_id::nothing};
    return action_queue_.front();
}

void unit_class::move(const int q, const int r, const bool queuing)
{
    if (t_m_ == 0)
    {
        const position p(q, r);
        const unit_action action(this->id_, this->player_, unit_action_id::move, p);
        if (queuing)
        {
            action_queue_.push(action);
        }
        else
        {
            const queue<unit_action> queue;
            action_queue_ = queue;
            action_queue_.push(action);
        }
    }
}

void unit_class::attack(object_abstract_class& b, const bool queuing)
{
    if (t_a_ == 0)
    {
        const unit_action action(this->id_, this->player_, unit_action_id::attack, b);
        if (queuing)
        {
            action_queue_.push(action);
        }
        else
        {
            const queue<unit_action> queue;
            action_queue_ = queue;
            action_queue_.push(action);
        }
    }
}

void unit_class::wait(const time_t t, const bool queuing)
{
    const unit_action action(this->id_, this->player_, unit_action_id::wait, t);
    if (queuing)
    {
        action_queue_.push(action);
    }
    else
    {
        const queue<unit_action> queue;
        action_queue_ = queue;
        action_queue_.push(action);
    }
}

void unit_class::follow(object_abstract_class& b, const bool queuing)
{
    if (t_m_ == 0)
    {
        const unit_action action(this->id_, this->player_, unit_action_id::follow, b);
        if (queuing)
        {
            action_queue_.push(action);
        }
        else
        {
            const queue<unit_action> queue;
            action_queue_ = queue;
            action_queue_.push(action);
        }
    }
}

void unit_class::pick(const food_class& food, const bool queuing)
{
    if (t_m_ == 0)
    {
        const unit_action action(this->id_, this->player_, unit_action_id::pick, food);
        if (queuing)
        {
            action_queue_.push(action);
        }
        else
        {
            const queue<unit_action> queue;
            action_queue_ = queue;
            action_queue_.push(action);
        }
    }   
}

void unit_class::update_cooldown()
{
    if (pathfinding_cooldown_ > 0)
        pathfinding_cooldown_--;
    else
        pathfinding_cooldown_ = 0;
    if (t_m_ > 0)
        t_m_--;
    else
        t_m_ = 0;
    if (t_a_ > 0)
        t_a_--;
    else
        t_a_ = 0;
}

void unit_class::path_set(const std::vector<hex_tile*>& path)
{
    path_ = path;
}

std::vector<hex_tile*>* unit_class::path_get()
{
    return &path_;
}

bool unit_class::carry_food_get() const
{
    return carry_food_;
}

void unit_class::carry_food_set(const bool c)
{
    carry_food_ = c;
}

void unit_class::temporary_position_set(const int q, const int r)
{
    this->temporary_p_.q_set(q);
    this->temporary_p_.r_set(r);
}

void unit_class::temporary_position_apply()
{
    if (temporary_p_.q_get() != -1 && temporary_p_.r_get() != -1)
        location_ = temporary_p_;
}

int unit_class::damage_per_frame_get() const
{
    return damage_per_frame_;
}

int unit_class::max_speed_get() const
{
    return max_speed_;
}
