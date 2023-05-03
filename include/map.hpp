#pragma once

#include <vector>
#include "player.hpp"
#include "units.hpp"

using namespace std;

class tile {
public:
    tile(bool passable) {
        this->passable = passable;
    };
    bool passable;
};