#include <vector>
#include <cmath>
#include <fstream>

#include "game_class.hpp"
#include "state_class.hpp"
#include "player_class.hpp"
#include "player_group.hpp"
#include "player_random_total.hpp"
#include "player_random_constant.hpp"
#include "player_mcts.hpp"
#include "player_dpf.hpp"

using namespace std;

void tokenize(std::string const& str, const char* delimiter, std::vector<std::string>& out)
{
    char* token = strtok(const_cast<char*>(str.c_str()), delimiter);
    while (token != nullptr)
    {
        out.emplace_back(token);
        token = strtok(nullptr, delimiter);
    }
} 


void game_players_init(state_class* state, game_class* game, const std::string& parameter_file)
{
    fstream file{parameter_file};
    string line;
    player_id player_id_order_check = 0;
    while (std::getline(file, line))
    {
        if (!line.empty() && line.c_str()[0] != '#')
        {
            const auto delimiter = " "; 
            vector<string> tokens;
            tokenize(line, delimiter, tokens);
            if (!tokens.empty())
            {
                if (string player = "player"; tokens[0] == player)
                {
                    if (player_id_order_check != stoi(tokens[2]))
                    {
                        BOOST_LOG_TRIVIAL(error) << "players not in order";
                    }
                    virtual_player_class* player_temp;
                    if (tokens[1] == "nothing")
                        player_temp = new player_nothing(stoi(tokens[2]));
                    if (tokens[1] == "rand_total")
                        player_temp = new player_rand_total(stoi(tokens[2]));
                    else if (tokens[1] == "rand_constant")
                        player_temp = new player_rand_constant(stoi(tokens[2]));
                    else if (tokens[1] == "group")
                        player_temp = new player_group(stoi(tokens[2]));
                    else if (tokens[1] == "dpf")
                        player_temp = new player_dpf(stoi(tokens[2]));
                    else if (tokens[1] == "mcts")
                    {
                        player_temp = new player_mcts(stoi(tokens[2]));
                        dynamic_cast<player_mcts*>(player_temp)
                            ->player_mcts_init(game, stoi(tokens[7]), stoi(tokens[8]), stoi(tokens[9]),
                                               stoi(tokens[10]), tokens[11] == "multi_thread", stoi(tokens[12]));
                    }
                    else
                    {
                        BOOST_LOG_TRIVIAL(error)
                            << "unknown player " << tokens[2] << " type: " << tokens[1];
                        player_temp = new player_rand_total(stoi(tokens[2]));
                    }
                    game->player_set(player_temp, stoi(tokens[2]));
                    state->unit_list_add();
                    for (int i = 0; i < stoi(tokens[3]); i++)
                    {
                        int q_pos;
                        int r_pos;
                        do
                        {
                            const double angle = (static_cast<double>(game->options_get().rand_n_get()) / mt19937::max()) * 2 * 3.1415;
                            const double radius =
                                (static_cast<double>(game->options_get().rand_n_get()) / mt19937::max()) * 10;
                            q_pos = stoi(tokens[5]) + static_cast<int>(radius * cos(angle));
                            r_pos = stoi(tokens[6]) + static_cast<int>(radius * sin(angle));
                        } while (!map_class::in_map(q_pos, r_pos, *game->map_get()) &&
                                 !game->map_get()->passable(q_pos, r_pos));

                        state->unit_new(q_pos, r_pos, player_id_order_check, unit_hp);
                    }
                    for (int i = 0; i < stoi(tokens[4]); i++)
                    {
                        state->base_new(stoi(tokens[5]), stoi(tokens[6]), player_id_order_check);
                    }
                    player_id_order_check++;
                }
            }
        }
    }
}
