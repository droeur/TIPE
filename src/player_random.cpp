#include "player_random.hpp"

vector<unitAction> randPlayer::getMoves(state &s, vector<vector<unitAction>> & possibleActionsVec){
    vector<unitAction> choisi;
    int taille = s.unitList_get()[(int)_playerID].size();
    int unitIndex = 0;
    while(unitIndex < taille){
        if(possibleActionsVec[unitIndex].size() > 0){
            int r = (rand() % possibleActionsVec[unitIndex].size());
            unitAction actionTempChoisie = possibleActionsVec[unitIndex][r];
            choisi.push_back(actionTempChoisie);
        }
        unitIndex++;
    }
    return choisi;
}