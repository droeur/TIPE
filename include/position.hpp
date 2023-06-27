#pragma once

#include <math.h>
#include "hex_map.hpp"

class position{
public:
    int distance(position p, map_class *map){
        return map->get_tile(_q, _r)->distance(map->get_tile(p._q, p._r));
    }
    int getXGraphic(map_class *map){
        return map->get_tile(_q, _r)->graphic_x();
    }
    int getYGraphic(map_class *map){
        return map->get_tile(_q, _r)->graphic_y();
    }
    int getQ(){
        return _q;
    }
    int getR(){
        return _r;
    }
    void setQ(int q){
        _q = q;
    }
    void setR(int r){
        _r = r;
    }
    void addQ(int q){
        _q += q;
    }
    void addR(int r){
        _r += r;
    }

    bool operator==(position p){
        return this->_q == p._q && this->_r == p._r;
    }
protected:
    int _q = 0, _r = 0;
};