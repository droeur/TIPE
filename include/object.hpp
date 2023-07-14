#pragma once

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
protected:
    position location_;
    int hit_point_ = 0;
    player_id player_ = -1;
    int id_ = -1;
    object_type type_ = object_type::undefined;
    int temporary_hp_ = 0;

public:
    virtual ~object_abstract_class() = default;

    object_abstract_class(const int q, const int r, const object_type t)
    {
        this->location_.q_set(q);
        this->location_.r_set(r);
        this->type_ = t;
    }

    object_abstract_class(const int q, const int r, const int hp, const object_type t)
    {
        this->location_.q_set(q);
        this->location_.r_set(r);
        this->hit_point_ = hp;
        this->type_ = t;
        this->temporary_hp_ = hp;
    }

    object_abstract_class(const int q, const int r, const int hp, const player_id id, const object_type t)
    {
        this->location_.q_set(q);
        this->location_.r_set(r);
        this->hit_point_ = hp;
        this->player_ = id;
        this->type_ = t;
        this->temporary_hp_ = hp;
    }

    [[nodiscard]] virtual object_type object_type_get() const
    {
        return this->type_;
    }

    virtual void position_set(const int q, const int r)
    {
        this->location_.q_set(q);
        this->location_.r_set(r);
    }

    [[nodiscard]] virtual int q_get() const
    {
        return this->location_.q_get();
    }

    [[nodiscard]] virtual int r_get() const
    {
        return this->location_.r_get();
    }

    virtual position &position_get()
    {
        return this->location_;
    }

    virtual void hp_set(const int hp)
    {
        this->hit_point_ = hp;
    }

    virtual void hp_remove(const int hp)
    {
        this->hit_point_ -= hp;
    }

    [[nodiscard]] virtual int hp_get() const
    {
        return this->hit_point_;
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

    virtual void temporary_hp_set(const int hp);
    virtual void temporary_hp_remove(const int hp);
    virtual void temporary_hp_apply();
    virtual int temporary_hp_get();
};
