#include <vector>
#include "game.hpp"
#include "state.hpp"
#include "units.hpp"

void state::moves_generate(){
    vector<uActionID> v{uActionID::ERROR, uActionID::MOVE, uActionID::ATTACK, uActionID::WAIT};
    for(int player = 0; player < NUMBER_OF_PLAYERS; player++){
        for(unit& u : _U_list[player]){
            vector<unitAction> *actionList = u.getPossibleActionVector();
            if(actionList->empty()){
                
            }
        }
    }
}
void state::moves_make(){

}