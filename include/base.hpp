#pragma once

#include "object.hpp"
#include "player.hpp"

class base_class : public object_abstract_class{
public:
    base_class(int q, int r) : object_abstract_class(q,r){

    };
private:
    PlayerID _joueur;
};