#include <iostream>
#include <vector>
#include <rapidcsv.h>
#include <memory>
#include <GLFW/glfw3.h>

#include "game.hpp"
#include "player.hpp"
#include "player_random.hpp"

using namespace std;

#include "graphics.hpp"

int main() {
    GLFWwindow* window = graphic_init();

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

    game g;
    state s{map, &g};
    vector<vector<unitAction>(*)(state *s, vector<unit> U)> P;
    unit u1{0,0};
    unit u2{1,1};
    vector<unit> unitlist1{u1};
    vector<unit> unitlist2{u2};
    s.unitList_add(unitlist1);
    s.unitList_add(unitlist2);
    while (!glfwWindowShouldClose(window)) {
        g.play(&s, P);
        draw_screen(s, window);
    }

    graphic_exit();
}