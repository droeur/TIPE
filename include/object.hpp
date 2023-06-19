#pragma once

#include <vector>
#include <stdexcept>


using namespace std;
#include "position.hpp"

typedef unsigned char PlayerID;

class object_abstract_class{
public:
    object_abstract_class(int q, int r){
        p.setQ(q);
        p.setR(r);
    }
    object_abstract_class(int q, int r, int hp){
        p.setQ(q);
        p.setR(r);
        this->_HP = hp;
    }
    object_abstract_class(int q, int r, int hp, PlayerID id){
        p.setQ(q);
        p.setR(r);
        this->_HP = hp;
        this->_joueur = id;
    }
    virtual void position_set(int q, int r) {
        vector<int> a = {0};
        if(q > 100 || r > 100){
            cout << this;
            a[3] = 10;
        }
        this->p.setQ(q);
        this->p.setR(r);
    }
    virtual double getQ(){
        return this->p.getQ();
    }
    virtual double getR(){
        return this->p.getR();
    }
    virtual position position_get(){
        return this->p;
    }
    virtual void setHP(int hp){
        this->_HP = hp;
    }
    virtual void removeHP(int hp){
        this->_HP -= hp;
    }
    virtual int getHP(){
        return this->_HP;
    }
    virtual PlayerID getPlayer(){
        return this->_joueur;
    }
    virtual void setPlayer(PlayerID joueur){
        this->_joueur = joueur;
    }
protected:
    position p;
    int _HP;
    PlayerID _joueur;
};