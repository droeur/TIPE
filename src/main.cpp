#include <iostream>
#include <vector>
#include <rapidcsv.h>
#include <memory>
#include "game.hpp"
#include "player.hpp"
#include "player_random.hpp"

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
    vector<shared_ptr<player>> P;

}