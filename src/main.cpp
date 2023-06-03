#include <iostream>
#include <vector>
#include <rapidcsv.h>
#include <memory>
#include <stdio.h>
#include <yaml-cpp/yaml.h>

#include "game.hpp"
#include "hex_map.hpp"
#include "player.hpp"
#include "player_random.hpp"
#include "test.hpp"

using namespace std;

#include "graphics.hpp"


int main() {
    YAML::Node config = YAML::LoadFile("config.yaml");
    if (config["player1"]) {
        for(YAML::const_iterator u_yaml=config.begin();u_yaml != config.end();++u_yaml){
            
        }
    }

    hex_tile tile();
    graphic game_graphic;
    // Load map
    rapidcsv::Document map_doc("map.csv", rapidcsv::LabelParams(-1, -1));
    map_class map;
    for(int q = 0; q < map_doc.GetColumnCount(); q++){
        vector<hex_tile> column;
        for(int r = 0; r < map_doc.GetColumn<int>(q).size(); r++) {
            bool passable = map_doc.GetColumn<int>(q).at(r) != 0;
            hex_tile t{q-(r + (r & 1))/2,r,passable};
            column.push_back(t);
        }
        map.add_column(column);
    }


    game g;
    state s{map, &g};
    vector<player*> P;
    player *tempPlayer;

    randPlayer player1((PlayerID)0);
    tempPlayer = &player1;
    P.push_back(tempPlayer);

    randPlayer player2((PlayerID)1);
    tempPlayer = &player2;
    P.push_back(tempPlayer);
    unit u11{0, 0, (PlayerID)0, 100};
    unit u21{0, 20, (PlayerID)1, 100};
    vector<unit> unitlist1{u11};
    vector<unit> unitlist2{u21};
    s.unitList_add(unitlist1);
    s.unitList_add(unitlist2);


    hex_tile start{1,0}, end{0,25};
    hex_tile start2{5,5}, end2{2,9};
    vector<hex_tile*> chemin = map.chemin(start, end);
    // vector<hex_tile*> chemin2 = map.chemin(start2, end2);
    game_graphic.dessin(s, map, chemin);
    // game_graphic.dessin(s, map, chemin2);


    //boucle du jeu
    bool quit = false; 
    while(!quit){
        uint64_t beginFrame = SDL_GetPerformanceCounter();
        g.play(s, P);
        quit = game_graphic.update(s);
        uint64_t endFrame = SDL_GetPerformanceCounter();
        double elapsed = (endFrame - beginFrame) / (double)SDL_GetPerformanceFrequency();
        s.fps_set(1.0/elapsed);
        cout << s.fps_get() << endl;
    }
    game_graphic.quit();
    return EXIT_SUCCESS;
}