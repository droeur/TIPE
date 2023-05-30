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
            hex_tile t{q-r/2,r};
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
    unit u11{static_cast<int>(rand()%500),static_cast<int>(rand()%500), (PlayerID)1, static_cast<int>(rand()%500)};
    unit u12{static_cast<int>(rand()%500),static_cast<int>(rand()%500), (PlayerID)1, static_cast<int>(rand()%500)};
    unit u13{static_cast<int>(rand()%500),static_cast<int>(rand()%500), (PlayerID)1, static_cast<int>(rand()%500)};
    unit u21{static_cast<int>(rand()%500),static_cast<int>(rand()%500), (PlayerID)1, static_cast<int>(rand()%500)};
    unit u22{static_cast<int>(rand()%500),static_cast<int>(rand()%500), (PlayerID)1, static_cast<int>(rand()%500)};
    unit u23{static_cast<int>(rand()%500),static_cast<int>(rand()%500), (PlayerID)1, static_cast<int>(rand()%500)};
    vector<unit> unitlist1{u11, u12, u13};
    vector<unit> unitlist2{u21, u22, u23};
    s.unitList_add(unitlist1);
    s.unitList_add(unitlist2);


    hex_tile start{0,0}, end{-3,6};
    hex_tile start2{5,5}, end2{2,8};
    vector<hex_tile*> chemin = map.chemin(start, end);
    // vector<hex_tile*> chemin2 = map.chemin(start2, end2);
    game_graphic.dessin(s, map, chemin);
    // game_graphic.dessin(s, map, chemin2);


    //boucle du jeu
    bool quit = false; 
    while(!quit){
        clock_t beginFrame = clock();
        g.play(s, P);
        quit = game_graphic.update(s);
        clock_t endFrame = clock();
        s.fps_set(1000.0/(endFrame-beginFrame));
    }
    game_graphic.quit();
    return EXIT_SUCCESS;
}