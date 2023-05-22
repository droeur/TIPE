#pragma once

#include <math.h>

class position{
public:
    double distance(position p){
        double x = _x - p._x;
        double y = _y - p._y;
        double a;
        a = pow(x,2) + pow(y,2);
        a = sqrt(a);
        return a;
    }
    double getX(){
        return _x;
    }
    double getY(){
        return _y;
    }
    void setX(double x){
        _x = x;
    }
    void setY(double y){
        _y = y;
    }
    void addX(double x){
        _x += x;
    }
    void addY(double y){
        _y += y;
    }
protected:
    double _x, _y;
};