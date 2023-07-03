#include "game_class.hpp"
#include "hex_map.hpp"
#include "parser.hpp"
#include "init.hpp"


using namespace std;

#include "graphics.hpp"

int main(const int argc, char* argv[])
{
    ofstream result{"result.txt", ofstream::out};
    const auto settings = new options_class{argc, argv};
    map_class map{settings->map_file_get()};
    graphic_class* game_graphic = nullptr;
    if (settings->graphics_get())
        game_graphic = new graphic_class{settings->graphic_folder_get(), settings->font_file_get()};
    for (int i = 0; i < settings->n_test_get(); i++)
    {
        cout << i << endl;
        const auto state = new state_class;
        const auto game = new game_class{&map, state};
        bool quit = false;

        game_players_init(state, game, settings->parameter_file_get());

        while (!quit)
        {
            state->fps_check_before();

            game->play();

            quit = game->winner_check();

            if (settings->graphics_get())
                quit = game_graphic->update(game) || quit;
            state->frame_increment();
            state->fps_check_after(settings->fast_get());
        }
        result << game->winner_get() << ",";
        delete state;
        delete game;
    }

    if (settings->graphics_get())
        delete game_graphic;

    delete settings;
    return EXIT_SUCCESS;
}
