#include <iostream>
#include <queue>
#include <algorithm>
#include <assert.h>
#include <float.h>
#include "hex_map.hpp"

using namespace std;

class Compare{
public:
    bool operator() (hex_tile *a, hex_tile *b){
        return a->cost_f > b->cost_f; 
    }
};


std::vector<hex_tile*> map_class::path_a_star(hex_tile* start, hex_tile* end)
{
    std::vector<hex_tile*> path;
    priority_queue<hex_tile*, vector<hex_tile*>, Compare> listeOuverte;
    bool listeFermee[size_q][size_r] = {true};
    hex_tile* listeEnfants[size_q][size_r];
    
    hex_tile *current_node = NULL;
    hex_tile *children = NULL;
    
    if(start == end){
        path.push_back(start);
        return path;
    }

    start->cost_f = 0.0;
    start->cost_g = 0.0;
    start->cost_h = 0.0;
    start->parent_q = start->q();
    start->parent_r = start->r();
    start->parent_s = start->s();

    listeOuverte.push(start);
    int i = 0;
    while(listeOuverte.size() > 0 && i < MAX_A_STAR_DEPTH) {
        current_node = listeOuverte.top();
        listeFermee[current_node->index_x()][current_node->index_y()] = true;
        listeEnfants[current_node->index_x()][current_node->index_y()] = current_node;
        listeOuverte.pop();
        for(int dir = 0; dir < 6; dir++){
            if(in_map(current_node->neighbor(dir).q(), current_node->neighbor(dir).r()) && get_tile(current_node->neighbor(dir).q(), current_node->neighbor(dir).r())->passable() && !listeFermee[current_node->neighbor(dir).index_x()][current_node->neighbor(dir).index_y()]){
                if(current_node->neighbor(dir).q() == end->q() && current_node->neighbor(dir).r() == end->r()){
                    path.push_back(end);
                    path.push_back(get_tile(current_node->q(), current_node->r()));
                    while(current_node != start){
                        hex_tile *t = get_tile(current_node->parent_q, current_node->parent_r);
                        path.push_back(t);
                        current_node = listeEnfants[current_node->parent_x()][current_node->parent_y()];
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
                children = get_tile(current_node->neighbor(dir).q(), current_node->neighbor(dir).r());
                children->cost_g = current_node->cost_g + 1.0;
                children->cost_h = children->distance(end);
                children->cost_f = children->cost_h + children->cost_g;
                children->parent_q = current_node->q();
                children->parent_r = current_node->r();
                children->parent_s = current_node->s();
                listeOuverte.push(children);
            }
        }
        listeFermee[current_node->index_x()][current_node->index_y()] = true;
        i++;
    }
    return path;
}