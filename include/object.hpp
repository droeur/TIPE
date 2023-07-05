#pragma once

#include <vector>

#include "position.hpp"

using player_id = int;

enum class object_type
{
    undefined,
    base,
    unit,
    food
};

enum
{
    base_hp = 100,
    unit_hp = 10
};

class object_abstract_class
{
public:
    virtual ~object_abstract_class() = default;

    object_abstract_class(const int q, const int r, const object_type t)
    {
        this->p_.q_set(q);
        this->p_.r_set(r);
        this->type_ = t;
    }

    object_abstract_class(const int q, const int r, const int hp, const object_type t)
    {
        this->p_.q_set(q);
        this->p_.r_set(r);
        this->hp_ = hp;
        this->type_ = t;
    }

    object_abstract_class(const int q, const int r, const int hp, const player_id id, const object_type t)
    {
        this->p_.q_set(q);
        this->p_.r_set(r);
        this->hp_ = hp;
        this->player_ = id;
        this->type_ = t;
    }

    [[nodiscard]] virtual object_type object_type_get() const
    {
        return this->type_;
    }

    virtual void position_set(const int q, const int r)
    {
        this->p_.q_set(q);
        this->p_.r_set(r);
    }

    [[nodiscard]] virtual int q_get() const
    {
        return this->p_.q_get();
    }

    [[nodiscard]] virtual int r_get() const
    {
        return this->p_.r_get();
    }

    virtual position position_get()
    {
        return this->p_;
    }

    virtual void hp_set(const int hp)
    {
        this->hp_ = hp;
    }

    virtual void hp_remove(const int hp)
    {
        this->hp_ -= hp;
    }

    [[nodiscard]] virtual int hp_get() const
    {
        return this->hp_;
    }

    [[nodiscard]] virtual player_id player_get() const
    {
        return this->player_;
    }

    virtual void player_set(const player_id player)
    {
        this->player_ = player;
    }

    [[nodiscard]] virtual int id_get() const
    {
        return this->id_;
    }

    virtual void id_set(const int id)
    {
        this->id_ = id;
    }

protected:
    position p_;
    int hp_ = 0;
    player_id player_ = -1;
    int id_ = -1;
    object_type type_ = object_type::undefined;
};
