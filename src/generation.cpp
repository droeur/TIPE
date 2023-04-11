#include <iostream>
#include "header.h"

using namespace std;

void generate_map(int map[MAPSIZE][MAPSIZE]){
    for(int i = 0; i < MAPSIZE; i++){
        for(int j = 0; j < MAPSIZE; j++)
            map[i][j] = 1;
    }
}