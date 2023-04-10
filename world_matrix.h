#pragma once
#include "constants.h"
typedef struct {
	tileType Matrix[MAPSIZE][MAPSIZE];
} MapLayer;


class world_matrix_class{
public:
	world_matrix_class();
	tileType tile_get(int i, int j);
private:
	MapLayer* layer;
};

typedef class world_matrix_class world_matrix;