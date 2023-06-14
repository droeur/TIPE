#pragma once

#include <vector>

using namespace std;
#include "position.hpp"

class object_abstract_class{
public:
    object_abstract_class(int q, int r){
        p.setQ(q);
        p.setR(r);
    }
    virtual void position_set(int q, int r) {
        this->p.setQ(q);
        this->p.setR(r);
    }
    virtual double getQ(){
        return this->p.getQ();
    }
    virtual double getR(){
        return this->p.getR();
    }
    virtual position position_get(){
        return this->p;
    }
protected:
    position p;
};