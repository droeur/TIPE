#pragma once

#include <vector>
#include "units.hpp"

using namespace std;

class tile {
public:
    tile(bool passable) {
        this->passable = passable;
    };
    bool passable;
};