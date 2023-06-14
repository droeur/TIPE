#include <vector>
#include "game.hpp"
#include "state.hpp"
#include "units.hpp"

void state::frame_increment(){
    this->_frame++;
}
frame state::frame_get(){
    return _frame;
}
void state::unitList_add(vector<unit> U){
    this->_U_list.push_back(U);
}
vector<vector<unit>> state::unitList_get(){
    return this->_U_list;
}
void state::unit_append(unit &u, PlayerID joueur){
    return this->_U_list[(int)joueur].push_back(u);
}

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
                        double distance = u->position_get().distance(ennemyU->position_get(), _map);
                        if(ATTACK_DISTANCE > distance){
                            u->attack(ennemyU);
                            _choosed_actions[player].erase(_choosed_actions[player].begin() + action_index);
                            u->getPath()->clear();
                        } else {
                            hex_tile start{u->position_get().getQ(), u->position_get().getR()};
                            hex_tile end{ennemyU->position_get().getQ(), ennemyU->position_get().getR()};
                            cout << u->getPath()->size() << endl;
                            if(u->getPath()->size() == 0){
                                vector<hex_tile*> chemin = _map->chemin(start, end);
                                u->setPath(chemin);
                            }
                            if(u->getPath()->size() > 1){
                                u->position_set((*u->getPath())[1]->q(), (*u->getPath())[1]->r());
                                u->getPath()->erase(u->getPath()->begin() + 1);
                            }
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