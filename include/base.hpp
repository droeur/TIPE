#pragma once

#include "object.hpp"
#include "player.hpp"

#define BASE_HP 100

class base_class : public object_abstract_class{
public:
    base_class(int q, int r, PlayerID joueur) : object_abstract_class(q,r, 100,joueur){

    };
    PlayerID playerId_get(){return _joueur;};

private:

};