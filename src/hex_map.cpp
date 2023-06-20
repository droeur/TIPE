#include <iostream>
#include <queue>
#include <algorithm>
#include <assert.h>
#include <float.h>
#include "hex_map.hpp"

class Compare{
public:
    bool operator() (hex_tile *a, hex_tile *b){
        return a->fCost > b->fCost; 
    }
};


vector<hex_tile*> map_class::chemin(hex_tile *start, hex_tile *end){
    vector<hex_tile*> path;
    priority_queue<hex_tile*, vector<hex_tile*>, Compare> listeOuverte;
    bool listeFermee[Q_TAILLE][R_TAILLE] = {true};
    hex_tile* listeEnfants[Q_TAILLE][R_TAILLE];
    
    hex_tile *current_node = NULL;
    hex_tile *children = NULL;
    
    if(start == end){
        path.push_back(start);
        return path;
    }

    start->fCost = 0.0;
    start->gCost = 0.0;
    start->hCost = 0.0;
    start->parentQ = start->q();
    start->parentR = start->r();
    start->parentS = start->s();

    listeOuverte.push(start);
    int i = 0;
    while(listeOuverte.size() > 0 && i < MAX_ASTAR_DEPTH) {
        current_node = listeOuverte.top();
        listeFermee[current_node->xIndex()][current_node->yIndex()] = true;
        listeEnfants[current_node->xIndex()][current_node->yIndex()] = current_node;
        listeOuverte.pop();
        for(int dir = 0; dir < 6; dir++){
            if(inMap(current_node->voisin(dir).q(), current_node->voisin(dir).r()) && get_tile(current_node->voisin(dir).q(), current_node->voisin(dir).r())->passable() && !listeFermee[current_node->voisin(dir).xIndex()][current_node->voisin(dir).yIndex()]){
                if(current_node->voisin(dir).q() == end->q() && current_node->voisin(dir).r() == end->r()){
                    path.push_back(end);
                    path.push_back(get_tile(current_node->q(), current_node->r()));
                    while(current_node != start){
                        hex_tile *t = get_tile(current_node->parentQ, current_node->parentR);
                        path.push_back(t);
                        current_node = listeEnfants[current_node->parentX()][current_node->parentY()];
                    }
                    reverse(path.begin(), path.end());
                    for(auto t : path){
                        vector<int> a = {0};
                        if(t->q() > 100 || t->r() > 100){
                            a[3] = 10;
                            cout << "error";
                        }
                    }
                    return path;
                }
                children = get_tile(current_node->voisin(dir).q(), current_node->voisin(dir).r());
                children->gCost = current_node->gCost + 1.0;
                children->hCost = children->distance(end);
                children->fCost = children->hCost + children->gCost;
                children->parentQ = current_node->q();
                children->parentR = current_node->r();
                children->parentS = current_node->s();
                listeOuverte.push(children);
            }
        }
        listeFermee[current_node->xIndex()][current_node->yIndex()] = true;
        i++;
    }
    return path;
}