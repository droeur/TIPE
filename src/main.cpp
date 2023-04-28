#include <iostream>
#include <vector>
#include <rapidcsv.h>
#include "game.hpp"

using namespace std;

int main() {
    // Load map
    rapidcsv::Document map_doc("map.csv", rapidcsv::LabelParams(-1, -1));
    vector<vector<tile>> map;
    for(int i = 0; i < map_doc.GetColumnCount(); i++){
        vector<tile> column;
        for(int j:map_doc.GetColumn<int>(i)) {
            tile t{(bool)j};
            column.push_back(t);
        }
        map.push_back(column);
    }
    // Print map
    for(vector<tile> i:map){
        for(tile j:i){
            cout << j.passable << " ";
        }
        cout << endl;
    }

    state s{map};
    vector<vector<action>(*)(state *s, vector<unit> U)> P;

    // initialize
    for(int i = 0; i < 2; i++){
        vector<unit> U;
        unit u_j{0,0};
        P.push_back(&player);
        U.push_back(u_j);
        s.unitList_add(U);
    }

    for(time_t t = 0; t < 1000; t++){
        game(&s, P);
    }
}