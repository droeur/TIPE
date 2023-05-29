#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <vector>

#define Q_TAILLE 10
#define R_TAILLE 10

using namespace std;
/**
 * Hex directions:
 *     3 2
 *    4 H 1
 *     5 6 
 */

class hex_tile {
public:
    hex_tile(int q, int r) : _q(q), _r(r), _s(-q-r){

    }
    hex_tile(int q, int r, int s) : _q(q), _r(r), _s(s){

    }
    hex_tile(int q, int r, int s, bool passable) : _q(q), _r(r), _s(s), _passable(passable){

    }
    hex_tile(int q, int r, bool passable) : _q(q), _r(r), _s(-q-r), _passable(passable){

    }
    bool operator== (hex_tile a) {
        return _q == a._q && _r == a._r && _s == a._s;
    }
    bool operator!= (hex_tile a) {
        return _q != a._q || _r != a._r || _s != a._s;
    }
    hex_tile operator+ (hex_tile a) {
        hex_tile b(_q + a._q, _r + a._r);
        return b;
    }
    hex_tile operator- (hex_tile a) {
        hex_tile b(_q - a._q, _r - a._r);
        return b;
    }

    bool passable(){
        return _passable;
    }

    int distance(hex_tile a){ // Manhattan distance
        hex_tile temp = *this - a;
        return (abs(temp._q) + abs(temp._r) + abs(temp._s)) / 2;
    }

    hex_tile direction(int dir){
        vector<hex_tile> hex_tile_directions = {
            hex_tile(1, 0), hex_tile(1, -1), hex_tile(0, -1), 
            hex_tile(-1, 0), hex_tile(-1, 1), hex_tile(0, 1)
        };
        assert (0 <= dir && dir < 6);
        return hex_tile_directions[dir];
    }
    hex_tile voisin(int dir){
        return *this + direction(dir);
    }
    
    int q(){
        return _q;
    }
    int r(){
        return _r;
    }
    int s(){
        return _s;
    }

protected:
    int _q = -1;
    int _r = -1;
    int _s = -1;
    bool _passable = false;
};

class map_class{
public:
    vector<hex_tile> chemin(hex_tile &start, hex_tile &end);
    void add_column(vector<hex_tile> &col){
        _tiles_map.push_back(col);
    }
private:
    int _x_size;
    int _y_size;
    vector<vector<hex_tile>> _tiles_map;
};