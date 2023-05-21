#include <vector>
#include "game.hpp"
#include "state.hpp"
#include "units.hpp"

void state::moves_generate(){
    vector<uActionID> v{uActionID::ERROR, uActionID::MOVE, uActionID::ATTACK, uActionID::WAIT};
    PlayerID ennemyPlayer;

    for(int player = 0; player < NUMBER_OF_PLAYERS; player++){
        _possibles_actions[player].clear();
        for(unit& u : _U_list[player]){
            vector<unitAction> *actualActions = u.getActualActionVector();

            if(actualActions->empty()){

                // On switch entre les différentes actions possibles
                if(u.canAttack()){
                    ennemyPlayer = g->getEnnemy(player);
                    for(unit& ennemyu : _U_list[ennemyPlayer]){
                        unitAction action{&u, uActionID::ATTACK, &ennemyu, ATTACK_COOLDOWN};
                        _possibles_actions[player].push_back(action);
                    }

                }

            }

        }

    }
}
void state::moves_make(){

}