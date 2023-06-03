#include <iostream>
#include <queue>
#include <algorithm>
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
    float gCost = FLT_MAX; //couts
    float hCost = FLT_MAX; //heuristique
    float fCost = FLT_MAX; //g+h

    int parentX(){
        return parentR;
    }
    int parentY(){
        return parentQ + (parentR + (parentR & 1)) / 2;
    }

    bool operator< (const pathfinding_node &a){
        return this->fCost < a.fCost;
    }
    bool operator> (const pathfinding_node &a){
        return this->fCost > a.fCost;
    }
    explicit operator hex_tile(){
        return hex_tile(this->_q, this->_r);
    }
    bool estValide(){
        return this->_passable;
    }
};
class Compare{
public:
    bool operator() (pathfinding_node *a, pathfinding_node *b){
        return a->fCost > b->fCost; 
    }
};


vector<hex_tile*> map_class::chemin(hex_tile &start, hex_tile &end){
    vector<hex_tile*> path;
    priority_queue<pathfinding_node*, vector<pathfinding_node*>, Compare> listeOuverte;
    bool listeFermee[Q_TAILLE][R_TAILLE] = {true};
    pathfinding_node* listeEnfants[Q_TAILLE][R_TAILLE];

    pathfinding_node *depart = new pathfinding_node{start.q(), start.r(), start.passable()};
    pathfinding_node *arrivee = new pathfinding_node{end.q(), end.r(), end.passable()};
    pathfinding_node *current_node = NULL;
    pathfinding_node *temporary_node = NULL;
    pathfinding_node *children = NULL;

    int index = 0, current_index = 0, children_index = 0;
    
    if(start == end){
        path.push_back(&start);
        return path;
    }

    depart->fCost = 0.0;
    depart->gCost = 0.0;
    depart->hCost = 0.0;
    depart->parentQ = depart->q();
    depart->parentR = depart->r();
    depart->parentS = depart->s();

    listeOuverte.push(depart);

    while(listeOuverte.size() > 0) {
        current_node = listeOuverte.top();
        listeFermee[current_node->xIndex()][current_node->yIndex()] = true;
        listeEnfants[current_node->xIndex()][current_node->yIndex()] = current_node;
        listeOuverte.pop();
        for(int dir = 0; dir < 6; dir++){
            if(inMap(current_node->voisin(dir).q(), current_node->voisin(dir).r()) && get_tile(current_node->voisin(dir).q(), current_node->voisin(dir).r())->passable() && !listeFermee[current_node->voisin(dir).xIndex()][current_node->voisin(dir).yIndex()]){
                if(current_node->voisin(dir).q() == end.q() && current_node->voisin(dir).r() == end.r()){
                    path.push_back(&end);
                    path.push_back(get_tile(current_node->q(), current_node->r()));
                    while(*current_node != start){
                        hex_tile *t = get_tile(current_node->parentQ, current_node->parentR);
                        path.push_back(t);
                        current_node = listeEnfants[current_node->parentX()][current_node->parentY()];
                    }
                    reverse(path.begin(), path.end());
                    return path;
                }
                children = new pathfinding_node{current_node->voisin(dir).q(), current_node->voisin(dir).r(), current_node->voisin(dir).passable()};
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
    }
    return path;
}