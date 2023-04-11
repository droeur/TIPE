#include <iostream>
#include "header.h"

using namespace std;

int main() {
    int map[MAPSIZE][MAPSIZE] = {0};
    generate_map(map);
    
    
    for(int i = 0; i < MAPSIZE; i++){
        for(int j = 0; j < MAPSIZE; j++){
            cout << map[i][j] << " ";
        }
        cout << endl;
    }
    return 0;
}