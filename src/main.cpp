#include <vector>
#include <memory>

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
    // On lit le fichier parametres
    // settings_parse();

    // On lit et cree la carte
    map_class map{"map.txt"};

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
    unit u11{0, 0, 0, unit_hp};
    unit u12{4, 5, 0, unit_hp};
    unit u21{0, 20, 1, unit_hp};
    vector unit_list_1{u11, u12};
    vector unit_list_2{u21};
    s.unit_list_add(unit_list_1);
    s.unit_list_add(unit_list_2);

    base_class base1(0, 5, 0);
    base_class base2(0, 25, 1);
    s.base_append(base1);
    s.base_append(base2);

    if (argc > 1)
    {
        test_func(map, s, game_graphic);
    }
    else
    {

        //boucle du jeu
        bool quit = false;
        while (!quit)
        {
            uint64_t begin_frame = SDL_GetPerformanceCounter();

            g.play(s, P);
            quit = game_graphic.update(s);

            uint64_t end_frame_before = SDL_GetPerformanceCounter();
            double elapsed_ms_before =
                (end_frame_before - begin_frame) / static_cast<double>(SDL_GetPerformanceFrequency());
            SDL_Delay(static_cast<uint32_t>(floor(33.3333 - elapsed_ms_before)));
            uint64_t end_frame_after = SDL_GetPerformanceCounter();
            double elapsed_ms_after = (end_frame_after - begin_frame) / static_cast<double>(SDL_GetPerformanceFrequency());
            s.fps_set(1.0 / elapsed_ms_after);
        }
    }
    game_graphic.quit();
    return EXIT_SUCCESS;
}
