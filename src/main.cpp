#include <iostream>
#include <vector>
#include <memory>
#include <stdio.h>

#include "game.hpp"
#include "hex_map.hpp"
#include "player.hpp"
#include "player_random.hpp"
#include "parser.hpp"


using namespace std;

#include "graphics.hpp"
#include "test.hpp"

int main(int argc, char *argv[]) {
    // On lit le fichier parametres
    settings_parse();

    // On lit et cree la carte
    map_class map{"map.csv"};

    // Initialisation des graphismes SDL
    graphic game_graphic;

    // Classes jeu et etat
    game g;
    state s{&map, &g};

    // TODO: partie temporaire pour creer les unitees et les joueurs
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

    if(argc > 1){
        test_func(map, s, game_graphic);
    } else {

        //boucle du jeu
        bool quit = false; 
        while(!quit){
            uint64_t beginFrame = SDL_GetPerformanceCounter();
            g.play(s, P);
            quit = game_graphic.update(s);
            uint64_t endFrame = SDL_GetPerformanceCounter();
            double elapsed = (endFrame - beginFrame) / (double)SDL_GetPerformanceFrequency();
            s.fps_set(1.0/elapsed);
            // cout << s.fps_get() << endl;
        }
    }
    game_graphic.quit();
    return EXIT_SUCCESS;
}