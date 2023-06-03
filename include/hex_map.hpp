#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <vector>
#include <map>

#define Q_TAILLE 64
#define R_TAILLE 64

using namespace std;
/**
 * Hex directions:
 *     3 2
 *    4 H 1
 *     5 6 
 */

class hex_tile {
public:
    hex_tile(int q, int r) : _q(q), _r(r), _s(-q-r), _xGraphic(q + ((double)r)/2){

    }
    hex_tile(int q, int r, int s) : _q(q), _r(r), _s(s), _xGraphic(q + ((double)r)/2){

    }
    hex_tile(int q, int r, int s, bool passable) : _q(q), _r(r), _s(s), _xGraphic(q + ((double)r)/2), _passable(passable){

    }
    hex_tile(int q, int r, bool passable) : _q(q), _r(r), _s(-q-r), _xGraphic(q + ((double)r)/2), _passable(passable){

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

    float distance(hex_tile &a){
        hex_tile temp = *this - a;
        return (abs(temp._q) + abs(temp._r) + abs(temp._s)) / 2;
    }
    float distance(hex_tile *a){
        hex_tile temp = *this - *a;
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
    // coordonnées cube
    int q(){
        return _q;
    }
    int r(){
        return _r;
    }
    int s(){
        return _s;
    }

    //coordonnées "offset even-r"
    int xIndex(){
        return _r;
    }
    int yIndex(){
        return _q + (_r + (_r & 1))/2;
    }
    double xGraphic(){
        return _xGraphic;
    }
    double yGraphic(){
        return _r;
    }

protected:
    int _q = -1;
    int _r = -1;
    int _s = -1;
    bool _passable = false;
    double _xGraphic;
};

class coordinates{
public:
    coordinates(int q, int r) : q(q), r(r){};
    int q;
    int r;
    bool operator== (coordinates *a){
        return a->q == this->q && a->r == this->r;
    }
};

class map_class{
public:
    vector<hex_tile*> chemin(hex_tile &start, hex_tile &end);
    hex_tile *get_tile(int q, int r){
        return &_tiles_map[q + (r + (r & 1))/2][r];
    }
    void add_column(vector<hex_tile> &col){
        _tiles_map.push_back(col);
    }
    vector<vector<hex_tile>> getTilesMap(){
        return _tiles_map;
    }
    bool inMap(int q, int r){
        return q + (r + (r & 1))/2 >= 0 && r >= 0;
    }
private:
    int _x_size;
    int _y_size;
    vector<vector<hex_tile>> _tiles_map;
    map<coordinates, hex_tile*> _tiles_hash;
};