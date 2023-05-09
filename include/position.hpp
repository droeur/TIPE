#pragma once

#include <math.h>

class position{
public:
    double distance(position p){
        return sqrt(pow(_x - p._x, 2) + pow(_y - p._y, 2));
    }
    double x(){
        return _x;
    }
    double y(){
        return _y;
    }
protected:
    double _x, _y;
};