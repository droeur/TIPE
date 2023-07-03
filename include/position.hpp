#pragma once
#include "hex_map.hpp"

class position
{
public:
    position(const int q, const int r)
        : q_(q)
        , r_(r){}

    position() = default;
    int distance(const position p, const map_class* map) const
    {
        return static_cast<int>(map->tile_get(q_, r_)->distance(map->tile_get(p.q_, p.r_)));
    }

    int graphic_x_get(const map_class* map) const 
    {
        return static_cast<int>(map->tile_get(q_, r_)->graphic_x());
    }

    int graphic_y_get(const map_class* map) const 
    {
        return static_cast<int> (map->tile_get(q_, r_)->graphic_y());
    }

    [[nodiscard]] int q_get() const
    {
        return q_;
    }

    [[nodiscard]] int r_get() const
    {
        return r_;
    }

    void q_set(const int q)
    {
        q_ = q;
    }

    void r_set(const int r)
    {
        r_ = r;
    }

    void q_add(const int q)
    {
        q_ += q;
    }

    void r_add(const int r)
    {
        r_ += r;
    }

    static int x_get(const int q, const int r)
    {
        return q + (r + (r & 1)) / 2;
    }

    static int y_get(const int r)
    {
        return r;
    }

    bool operator==(const position p) const
    {
        return this->q_ == p.q_ && this->r_ == p.r_;
    }

protected:
    int q_ = 0, r_ = 0;
};
