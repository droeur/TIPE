#pragma once

#include <vector>
using namespace std;

enum class uType {};

class tile {
public:
    tile(bool passable) {
        this->passable = passable;
    };
    bool passable;
};

class unit {
public:
    unit(double x, double y){
        this->x = x;
        this->y = y;
    }
//actions
    void move(double x ,double y);
    void attack(unit* b);
    void wait(time_t t);

private:
    double x,y;
    int hp;
    int t_a, t_m;
    uType type;
};

class state {
public:
    state(vector<vector<tile>> map) {
        t = 0;
        this->map = map;
    };
    void time_update();
    time_t time_get();

    void unitList_add(vector<unit> U);
    vector<vector<unit>> unitList_get();
private:
    time_t t;
    vector<vector<unit>> U_list;
    vector<vector<tile>> map;
};

class action{
private:
    unit u;
    uType type;
    unit cible;
    time_t t;
};

vector<action> player(state *s, vector<unit> U);
void game(state *s, vector<vector<action>(*)(state *s, vector<unit> U)> P);