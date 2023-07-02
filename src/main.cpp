#include "game_class.hpp"
#include "hex_map.hpp"
#include "parser.hpp"
#include "init.hpp"


using namespace std;

#include "graphics.hpp"

int main(const int argc, char* argv[])
{
    const auto settings = new options_class{argc, argv};
    map_class map{settings->map_file_get()};
    const auto game_graphic =   new graphic_class{settings->graphic_folder_get(), settings->font_file_get()};
    const auto game         =   new game_class;
    const auto state        =    new state_class{&map, game};
    bool quit = false;

    game_players_init(state, game);

    while (!quit)
    {
        state->fps_check_before();

        game->play(state);

        quit = game->winner_check(state);

        quit = game_graphic->update(state) || quit;

        state->fps_check_after();
    }

    delete game_graphic;
    delete settings;
    return EXIT_SUCCESS;
}
