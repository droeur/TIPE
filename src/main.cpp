#include <vector>
#include <memory>
#include <fstream>

#include "game.hpp"
#include "hex_map.hpp"
#include "player.hpp"
#include "player_random.hpp"
#include "base.hpp"


using namespace std;

#include "graphics.hpp"
#include "test.hpp"

int main(int argc, char* argv[])
{
/*    ofstream result{"result.txt", ofstream::out};
    for (int i = 0; i < 100; i++)
    {*/
        player_id gagnant = -1;
        // cout << i << endl;
        // On lit le fichier parametres
        // settings_parse();

        // On lit et cree la carte
        map_class map{"map_empty.txt"};

        // Initialisation des graphismes SDL
        graphic game_graphic;

        // Classes jeu et etat
        game g;
        state s{&map, &g};

        // TODO: partie temporaire pour creer les unitees et les joueurs
        vector<player*> P;
        player* tempPlayer;

        randPlayer player1(0);
        tempPlayer = &player1;
        P.push_back(tempPlayer);

        randPlayer player2(1);
        tempPlayer = &player2;
        P.push_back(tempPlayer);

        unit u1{72, 50, 0, unit_hp};
        unit u2{-24, 50, 1, unit_hp};
        vector<unit> unit_list_1{u1, u1, u1, u1, u1};
        vector<unit> unit_list_2{u2, u2, u2, u2, u2};
        s.unit_list_add(unit_list_1);
        s.unit_list_add(unit_list_2);

        base_class base1(72, 50, 0);
        base_class base2(-24, 50, 1);
        s.base_append(base1);
        s.base_append(base2);

        if (argc > 1)
        {
          test_func(map, s, game_graphic);
        }
        else
        {

            // boucle du jeu
            bool quit = false;
            while (!quit)
            {
                uint64_t begin_frame = SDL_GetPerformanceCounter();

                g.play(s, P);
                for (base_class &b:*s.base_list_get())
                {
                    if (b.hp_get() <= 0 )
                    {
                        if (gagnant != -1)
                        {
                            gagnant = 2;
                        }
                        else
                        {
                            gagnant = g.getEnnemy(b.playerId_get());
                        }
                        quit = true;
                    }
                }
                int player = 0; 
                for (vector<unit>& u_l : s.unit_list_get())
                {
                    int sum = 0;
                    for (unit &u:u_l)
                    {
                        player = u.player_get();
                        sum += u.hp_get();
                    }
                    if (sum <= 0)
                    {
                        if (gagnant != -1)
                        {
                            gagnant = 2;
                        }
                        else
                        {
                            gagnant = g.getEnnemy(player);
                        }
                        quit = true;
                    }
                }

                quit = game_graphic.update(s);

                uint64_t end_frame_before = SDL_GetPerformanceCounter();
                double elapsed_ms_before =
                    (end_frame_before - begin_frame) / static_cast<double>(SDL_GetPerformanceFrequency());
                SDL_Delay(static_cast<uint32_t>(floor(33.3333 - elapsed_ms_before)));
                uint64_t end_frame_after = SDL_GetPerformanceCounter();
                double elapsed_ms_after =
                    (end_frame_after - begin_frame) / static_cast<double>(SDL_GetPerformanceFrequency());
                s.fps_set(1.0 / elapsed_ms_after);
            }
        }
        game_graphic.quit();
//        result << gagnant << ",";
//    }
    return EXIT_SUCCESS;
}
