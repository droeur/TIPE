#pragma once

#include "object.hpp"

class base_class final : public object_abstract_class{
public:
    base_class(const int q, const int r, const player_id player) : object_abstract_class(q,r, 100,player, object_type::base){

    }

    [[nodiscard]] player_id player_id_get() const {return player_;}
};