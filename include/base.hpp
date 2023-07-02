#pragma once

#include "object.hpp"
#include "player_class.hpp"

#define BASE_HP 100

class base_class : public object_abstract_class{
public:
    base_class(int q, int r, player_id joueur) : object_abstract_class(q,r, 100,joueur, object_type::base){

    };
    player_id playerId_get(){return player_;};

private:

};