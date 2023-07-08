#pragma once

#include "object.hpp"

class food_class final : public object_abstract_class{
public:
    food_class(const int q, const int r)
        : object_abstract_class(q, r, object_type::food)
    {}
};