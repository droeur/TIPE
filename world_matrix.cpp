#include <cstdlib>
#include <iostream>
#include "world_matrix.h"

using namespace std;

world_matrix_class::world_matrix_class() {
	int i, j, random;
	layer = (MapLayer*)malloc(sizeof(MapLayer));
	for (i = 0; i < MAPSIZE; i++) {
		for (j = 0; j < MAPSIZE; j++) {
			random = rand() % 3;
			switch (random) {
			default:
				layer->Matrix[i][j] = tileType::GRASS;
			case 1:
				layer->Matrix[i][j] = tileType::WATER;
			case 2:
				layer->Matrix[i][j] = tileType::SAND;
			}

		}
	}
}

tileType world_matrix_class::tile_get(int i, int j) {
	return (*layer).Matrix[i][j];
}