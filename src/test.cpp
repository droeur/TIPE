#include <iostream>
#include <assert.h>
#include "state.hpp"
#include "game.hpp"
#include "player.hpp"
#include "test.hpp"
#include "hex_map.hpp"
#include "graphics.hpp"

using namespace std;

void test_func(map_class &map, state &s, graphic &game_graphic){
    hex_tile *start = map.get_tile(0,0), *end = map.get_tile(0,5);
    hex_tile *start2 = map.get_tile(0,0), *end2 = map.get_tile(64,64);
    vector<hex_tile*> chemin = map.chemin(start, end);
    vector<hex_tile*> chemin2 = map.chemin(start2, end2);
    vector<hex_tile*> bon_chemin{   new hex_tile{1,0},
                                    new hex_tile{2,0},
                                    new hex_tile{2,1},
                                    new hex_tile{1,2},
                                    new hex_tile{0,3},
                                    new hex_tile{0,4},
                                    new hex_tile{0,5}};
    assert(chemin.size() == bon_chemin.size());
    bool bon = true;
    for(unsigned int i = 0; i < chemin.size(); i++){
        bon = bon && (*chemin[i] == *bon_chemin[i]);
    }
    assert(bon);
    assert(chemin2.empty());
    cout << "tests bons !" << endl;
}