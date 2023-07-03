#include <vector>

#include "game_class.hpp"
#include "state_class.hpp"
#include "player_class.hpp"
#include "player_random.hpp"

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


void game_players_init(state_class* s, game_class* g, const std::string& parameter_file)
{
    fstream file{parameter_file};
    string line;
    player_id player_id_order_check = 0;
    while (std::getline(file, line))
    {
        if (!line.empty() && line.c_str()[0] != '#')
        {
            auto delimiter = " "; 
            vector<string> tokens;
            tokenize(line, delimiter, tokens);
            if (!tokens.empty())
            {
                if (string player = "player"; tokens[0] == player)
                {
                    if (player_id_order_check != stoi(tokens[2]))
                    {
                        cerr << "Error: players not in order";
                    }
                    virtual_player_class* player_temp;
                    if (tokens[1] == "rand")
                    {
                        player_temp = new player_rand(stoi(tokens[2]));
                    }
                    else
                    {
                        player_temp = new player_rand(stoi(tokens[2]));
                    }
                    g->player_set(player_temp, stoi(tokens[2]));
                    vector<unit_class*> unit_list;
                    for (int i = 0; i < stoi(tokens[3]); i++)
                    {
                        auto u = new unit_class{stoi(tokens[5]), stoi(tokens[6]), player_id_order_check, unit_hp};
                        unit_list.push_back(u);
                    }
                    s->unit_list_add(unit_list);
                    for (int i = 0; i < stoi(tokens[4]); i++)
                    {
                        const auto base = new base_class{stoi(tokens[5]), stoi(tokens[6]), player_id_order_check};
                        s->base_append(base);
                    }
                    player_id_order_check++;
                }
            }
        }
    }
}