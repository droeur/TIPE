#ifndef PG_H_INCLUDED
#define PG_H_INCLUDED

#include "liste.h"

#define MAPSIZE 640

typedef struct {
    int x;
    int y;
} map_tile;

typedef struct WorldMap_struct WorldMap;

WorldMap* generateWorld();

#endif