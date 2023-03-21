#include <iostream>
#include <GLFW/glfw3.h>
using namespace std;

#include "env_world.h"
#include "constants.h"

typedef struct {
    float r;
    float g;
    float b;
}rgb_color;

void tile_to_color(tileType tile, rgb_color *color) {
    switch (tile) {
    case tileType::GRASS:
        color->r = 0.180;
        color->g = 0.750;
        color->b = 0.208;
        break;
    case tileType::WATER:
        color->r = 0.180;
        color->g = 0.560;
        color->b = 0.750;
        break;
    case tileType::SAND:
        color->r = 0.860;
        color->g = 0.848;
        color->b = 0.155;
        break;
    }
    return;
}

void env_world::print_world() {
	int i,j;
    rgb_color tileColor;
	for (i = 0; i < MAPSIZE; i++) {
		for (j = 0; j < MAPSIZE; j++) {
            tile_to_color(base_layer.tile_get(i, j), &tileColor);
            glColor3f(tileColor.r, tileColor.g, tileColor.b);
            glBegin(GL_QUADS);
                glVertex2f(-1.0 + (double)i*2 / MAPSIZE, 1.0 - (double)j * 2 / MAPSIZE);
                glVertex2f(-1.0 + (double)i*2 / MAPSIZE, -1.0 - (double)j * 2 / MAPSIZE);
                glVertex2f(1.0 + (double)i*2 / MAPSIZE, -1.0 - (double)j * 2 / MAPSIZE);
                glVertex2f(1.0 + (double)i*2 / MAPSIZE, 1.0 - (double)j * 2 / MAPSIZE);
            glEnd();
		}
	}
}