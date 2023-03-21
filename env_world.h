#pragma once
#include "world_matrix.h"
class env_world {
public:
	void print_world();
private:
	world_matrix city_layer;
	world_matrix unit_layer;
	world_matrix base_layer;
};

