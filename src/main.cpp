#include "game_class.hpp"
#include "hex_map.hpp"
#include "parser.hpp"
#include "init.hpp"

#include <fstream>
#include <boost/log/trivial.hpp>

using namespace std;

#include "graphics.hpp"

int main(const int argc, char* argv[])
{
    const auto settings = new options_class{argc, argv};
    if (!settings->check())
    {
        return EXIT_FAILURE;
    }
    map_class map{settings->map_file_get()};
    graphic_class* game_graphic = nullptr;
    if (settings->graphics_get())
        game_graphic = new graphic_class{settings->graphic_folder_get(), settings->font_file_get()};
    
    time_t raw_time;
    char buffer[20];

#ifdef _WIN32
    time(&raw_time);
    tm time_info{};
    localtime_s(&time_info, &raw_time);
    strftime(buffer, 20, "%y_%m_%d %H_%M_%S", &time_info);
#else
    raw_time = time(nullptr);
    struct tm* time_info = localtime(&raw_time);
    strftime(buffer, 20, "%y_%m_%d %H_%M_%S", time_info);
#endif

    const string output_file = settings->output_folder_get() + "out " + buffer + ".txt";
    ofstream result{output_file.c_str(), ofstream::out};
    int numb_of_win[4] = {0};
    player_type players[2];
    int i = 0;
    for (; i < settings->n_test_get(); i++)
    {
        BOOST_LOG_TRIVIAL(debug) << "Running test " << i;
        const auto state = new state_class;
        state->options_set(*settings);
        const auto game = new game_class{&map, state, settings};
        bool quit = false;

        game_players_init(state, game, settings->parameter_file_get());

        while (!quit)
        {
            state->fps_check_before();
            if (!settings->pause_get())
                game->play();

            if (!settings->graphics_get())
                quit = game->winner_check();
            else
                quit = game_graphic->update(game, settings);
            state->fps_check_after(settings->fast_get());
	}
        result << game->winner_get() << ",";
        if (game->winner_get() < 3 && game->winner_get() >= -1)
            numb_of_win[game->winner_get() + 1]++;

        players[0] = game->player_get(0).player_type_get();
        players[1] = game->player_get(1).player_type_get();

        BOOST_LOG_TRIVIAL(info) << "Game " << i+1 << "/" << settings->n_test_get() << " : winner " << game->winner_get()
                                << " " << state->evaluate(0) << "/" << state->evaluate(1) << " after " << state->frame_get()
                                << " ticks";
        delete state;
        delete game;
    }

    if (settings->graphics_get())
        delete game_graphic;

    result << endl << "(" << numb_of_win[0] << ") " << numb_of_win[1] << "/" << numb_of_win[2] << " draws:" << numb_of_win[3];
    result << endl << "win rate " << virtual_player_class::player_type_to_string(players[0]) << " (0):" << 100.0 * (static_cast<double>(numb_of_win[1]) / i);
    result << endl << "win rate " << virtual_player_class::player_type_to_string(players[1])
           << " (1):" << 100.0 * (static_cast<double>(numb_of_win[2]) / i);
    result.close();

    BOOST_LOG_TRIVIAL(info) << "Result outputted to " << output_file;
    cout << result.rdbuf();
    delete settings;
    return EXIT_SUCCESS;
}
