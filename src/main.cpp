#include <iostream>
#include <vector>
#include <rapidcsv.h>
#include "header.h"

using namespace std;

int main() {
    int map[MAPSIZE][MAPSIZE] = {0};
    vector<int> column;
    generate_map(map);
    
    
    rapidcsv::Document doc("map.csv", rapidcsv::LabelParams(-1, -1));
    
    for(int i = 0; i < 10; i++){
        column = doc.GetColumn<int>(i);
        for(int j: column){
            cout << j;
        }
        cout << endl;
    }

}