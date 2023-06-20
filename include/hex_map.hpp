#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <vector>
#include <rapidcsv.h>
#include <float.h>

#define Q_TAILLE 64
#define R_TAILLE 64
#define MAX_ASTAR_DEPTH Q_TAILLE * R_TAILLE

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
    hex_tile(int q, int r, int s, bool passable) : _q(q), _r(r), _s(s), _passable(passable), _xGraphic(q + ((double)r)/2){

    }
    hex_tile(int q, int r, bool passable) : _q(q), _r(r), _s(-q-r), _passable(passable), _xGraphic(q + ((double)r)/2){

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

    bool operator< (const hex_tile &a){
        return this->fCost < a.fCost;
    }
    bool operator> (const hex_tile &a){
        return this->fCost > a.fCost;
    }
    explicit operator hex_tile(){
        return hex_tile(this->_q, this->_r);
    }
    bool estValide(){
        return this->_passable;
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
    map_class(string file_name){
        rapidcsv::Document map_doc(file_name, rapidcsv::LabelParams(-1, -1));
        for(int q = 0; (unsigned int)q < map_doc.GetColumnCount(); q++){
            vector<hex_tile> column;
            for(int r = 0; (unsigned int)r < map_doc.GetColumn<int>(q).size(); r++) {
                bool passable = map_doc.GetColumn<int>(q).at(r) != 0;
                hex_tile t{q-(r + (r & 1))/2,r,passable};
                column.push_back(t);
            }
            this->add_column(column);
        }
    };
    vector<hex_tile*> chemin(hex_tile *start, hex_tile *end);
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
        return q + (r + (r & 1))/2 >= 0 && r >= 0 && q + (r + (r & 1))/2 < Q_TAILLE && r < R_TAILLE;
    }
    bool passable(int q, int r){
        if(!inMap(q,r))
            return false;
        return _tiles_map[q + (r + (r & 1))/2][r].passable();
    }
private:
    int _x_size;
    int _y_size;
    vector<vector<hex_tile>> _tiles_map;
    map<coordinates, hex_tile*> _tiles_hash;
};