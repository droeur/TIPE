#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "graph.h"
#include "liste.h"
#include "proceduralGeneration.h"

/***
 * ^ x
 * |  y
 * +--> 
*/

#define VORONOI_POINTS 5

typedef struct {
    int x;
    int y;
} VoronoiSite;

typedef struct {
    VoronoiSite* point;
} SiteEvent;

struct WorldMap_struct {
    map_tile tiles[MAPSIZE][MAPSIZE];
};

WorldMap* generateWorld(){
    GenericList *voronoiSitesList = GenericListCreate();
    GenericList *queue = GenericListCreate();
    WorldMap *map = (WorldMap*)calloc(1, sizeof(WorldMap));
    VoronoiSite *site;
    int i,j;
    int y_d;
    for(i = 0; i < VORONOI_POINTS; i++){
        for(j = 0; j < VORONOI_POINTS; j++){
            site = (VoronoiSite*)calloc(1, sizeof(VoronoiSite));
            site->x = rand() % MAPSIZE;
            site->y = rand() % MAPSIZE;
            GenericNodeAppend(voronoiSitesList, site);
        }
    }

    for(y_d; y_d < MAPSIZE; y_d++){ // passage de la sweep line
        site = (VoronoiSite*)GenericNodeGet(voronoiSitesList, 0);
        for(i = 0; site; i++){ // check si la sweep line passe par un site
            if(y_d == site->y){
                
            }

            site = (VoronoiSite*)GenericNodeGet(voronoiSitesList, i);
        }
    }

    for(i = 0; i < VORONOI_POINTS; i++){
        for(j = 0; j < VORONOI_POINTS; j++){
            site = (VoronoiSite*)GenericNodeGet(voronoiSitesList, VORONOI_POINTS*i + j);
            printf("%d %d\n", site->x, site->y);
        }
    }
    return map;
}