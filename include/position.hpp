#pragma once

#include <math.h>
#include "hex_map.hpp"

class position
{
public:
    int distance(const position p, map_class* map) const
    {
        return map->tile_get(q_, r_)->distance(map->tile_get(p.q_, p.r_));
    }

    int getXGraphic(map_class* map)
    {
        return map->tile_get(q_, r_)->graphic_x();
    }

    int getYGraphic(map_class* map)
    {
        return map->tile_get(q_, r_)->graphic_y();
    }

    int getQ()
    {
        return q_;
    }

    int getR()
    {
        return r_;
    }

    void setQ(int q)
    {
        q_ = q;
    }

    void setR(int r)
    {
        r_ = r;
    }

    void addQ(int q)
    {
        q_ += q;
    }

    void addR(int r)
    {
        r_ += r;
    }

    static int x_get(int q_, int r_)
    {
        return q_ + (r_ + (r_ & 1)) / 2;
    }

    static int y_get(int q, int r)
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
