#include <iostream>
#include <vector>
#include <rapidcsv.h>
#include <memory>
#include <stdio.h>

#include "game.hpp"
#include "player.hpp"
#include "player_random.hpp"

using namespace std;

#include "graphics.hpp"


int main() {
    graphic game_graphic;
    // Load map
    rapidcsv::Document map_doc("map.csv", rapidcsv::LabelParams(-1, -1));
    vector<vector<tile>> map;
    for(int i = 0; i < map_doc.GetColumnCount(); i++){
        vector<tile> column;
        for(int j:map_doc.GetColumn<int>(i)) {
            tile t{(bool)j};
            column.push_back(t);
        }
        map.push_back(column);
    }
    // Print map
    for(vector<tile> i:map){
        for(tile j:i){
            cout << j.passable << " ";
        }
        cout << endl;
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
    unit u11{10.0,50.0, (PlayerID)0};
    unit u12{30.0,70.0, (PlayerID)0};
    unit u13{50.0,40.0, (PlayerID)0};
    unit u14{70.0,40.0, (PlayerID)0};
    unit u21{10.0,20.0, (PlayerID)1};
    unit u22{30.0,20.0, (PlayerID)1};
    unit u23{50.0,20.0, (PlayerID)1};
    unit u24{70.0,20.0, (PlayerID)1};
    vector<unit> unitlist1{u11, u12, u13, u14};
    vector<unit> unitlist2{u21, u22, u23, u24};
    s.unitList_add(unitlist1);
    s.unitList_add(unitlist2);

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