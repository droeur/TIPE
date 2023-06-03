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
            vector<unitAction> thisUnitAction;

            // On switch entre les différentes actions possibles
            if(u.canAttack()){
                ennemyPlayer = g->getEnnemy(player);
                for(unit& ennemyu : _U_list[ennemyPlayer]){
                    if(ennemyu.getHP() > 0){
                        unitAction action{&u, uActionID::ATTACK, &ennemyu, ATTACK_COOLDOWN};
                        thisUnitAction.push_back(action);
                    }
                }
            }
            if(u.canMove()){
            }
            _possibles_actions[player].push_back(thisUnitAction);

        }

    }
}

/**
 * @brief Pour update tout -> fait les actions et réduits les cooldowns
 * 
 */
void state::moves_make(){
    for(int player = 0; player < NUMBER_OF_PLAYERS; player++){
        if(_choosed_actions[player].size()){
            int action_index = 0;
            for(unitAction action:_choosed_actions[player]){
                unit *u = action.unit_get();
                switch(action.actionType_get()){
                case uActionID::ATTACK:
                    {
                        unit *ennemyU = action.targetUnit_get();
                        double distance = u->position_get().distance(ennemyU->position_get(), &_map);
                        if(ATTACK_DISTANCE > distance){
                            u->attack(ennemyU);
                            _choosed_actions[player].erase(_choosed_actions[player].begin() + action_index);
                        } else {
                            hex_tile start{u->position_get().getQ(), u->position_get().getR()};
                            hex_tile end{ennemyU->position_get().getQ(), ennemyU->position_get().getR()};
                            vector<hex_tile*> chemin = _map.chemin(start, end);
                            u->position_set(chemin[1]->q(), chemin[1]->r());
                        }
                    }
                    break;
                default:
                    cout << "Error : invalid action to do in non-zero vec" << endl;
                }
                action_index++;
            }
        }
        for(unit &u:_U_list[player]){
            u.update_coolDown();
        }
    }
}