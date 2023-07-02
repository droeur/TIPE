#include <vector>

#include "game_class.hpp"
#include "state_class.hpp"
#include "player_class.hpp"
#include "player_random.hpp"

using namespace std;

void game_players_init(state_class* s, game_class *g){
    player_class* tempPlayer;

    tempPlayer = new randPlayer(0);
    g->player_set(tempPlayer, 0);

    tempPlayer = new randPlayer(1);
    g->player_set(tempPlayer, 1);

    unit u1{0, 5, 0, unit_hp};
    unit u2{0, 10, 1, unit_hp};
    vector<unit> unit_list_1{u1, u1, u1, u1, u1};
    vector<unit> unit_list_2{u2, u2, u2, u2, u2};
    s->unit_list_add(unit_list_1);
    s->unit_list_add(unit_list_2);

    base_class base1(0, 5, 0);
    base_class base2(0, 10, 1);
    s->base_append(base1);
    s->base_append(base2);
}