#include <iostream>
#include <array>
#include <assert.h>
#include <float.h>
#include "hex_map.hpp"

class pathfinding_node : public hex_tile {
public:
    pathfinding_node(int q, int r, bool passable) : hex_tile(q,r,passable){

    }
    int parentQ = -1;
    int parentR = -1;
    int parentS = -1;
    float gCost = FLT_MAX;
    float hCost = FLT_MAX;
    float fCost = FLT_MAX;

    inline bool operator< (const pathfinding_node &a){
        return this->fCost < a.fCost;
    }
    explicit operator hex_tile(){
        return hex_tile(this->_q, this->_r);
    }
    bool estValide(){
        return this->_passable;
    }
};

vector<hex_tile> map_class::chemin(hex_tile &start, hex_tile &end){
    vector<hex_tile> vide;
    vector<pathfinding_node> listeOuverte;
    bool listeFermee[Q_TAILLE][R_TAILLE];
    vector<pathfinding_node> listeEnfants;
    if(end == start)
        return vide;
    if(end.passable())
        return vide;
    vector<vector<pathfinding_node>> allMap;
    for(int q = 0; q < Q_TAILLE; q++){
        for(int r = 0; r < R_TAILLE; r++){
            pathfinding_node node{q,r,true};
            allMap[q][r] = node;
        }
    }
    int q = start.q();
    int r = start.r();
    allMap[q][r].fCost = 0.0;
    allMap[q][r].gCost = 0.0;
    allMap[q][r].hCost = 0.0;
    allMap[q][r].parentQ = q;
    allMap[q][r].parentQ = r;
    listeOuverte.emplace_back(allMap[q][r]);
    while(!listeOuverte.empty()){
        pathfinding_node *node;
        do {
            float tempMax = FLT_MAX;
            vector<pathfinding_node>::iterator itNode;
            //recupere l'element le plus petit
            for(vector<pathfinding_node>::iterator it = listeOuverte.begin();
                    it != listeOuverte.end(); it = next(it)){
                pathfinding_node n = *it;
                if (n.fCost < tempMax){
                    tempMax = n.fCost;
                    node = &n;
                }
            }
            listeOuverte.erase(itNode);
        } while(node->estValide() == false);
    }
    return vide;
}