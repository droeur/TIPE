#include "player_random.hpp"

using namespace std;

vector<unit_action> randPlayer::getMoves(state_class *s, vector<vector<unit_action>> & possibleActionsVec){
    vector<unit_action> choisi;
    int taille = s->unit_list_get()[(int)_playerID].size();
    int unitIndex = 0;
    while(unitIndex < taille){
        if(possibleActionsVec[unitIndex].size() > 0){
            int r = (rand() % possibleActionsVec[unitIndex].size());
            unit_action actionTempChoisie = possibleActionsVec[unitIndex][r];
            choisi.push_back(actionTempChoisie);
        }
        unitIndex++;
    }
    return choisi;
}