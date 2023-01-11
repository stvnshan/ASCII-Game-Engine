#ifndef OBJECT_H
#define OBJECT_H
#include <memory>
#include <vector>
#include "state.h"
//#include "triple.h"
#include "movement.h"
#include "obstate.h"

class Object{
    int ID;
    int height;
    std::vector<Triple> tri;
    State obtype;
    int hp;
    int changeMv;
    int changeProg = 0;
    ObState obstate;

    
public: 

    std::vector<std::unique_ptr<Movement>> mv;
    
    Object( int ID, int height, std::vector<Triple> tri, State obtype ,int hp=0, int changeMv = 0, ObState obstate= ObState::NORMAL);


    int getID();
    
    void setChangeMv(int changeMv);
    int getChangeMv();

    void setChangeProg(int changeProg);
    int getchangeProg();

    void setHp(int hp);
    int getHp();

    void setHeight(int height);
    int getHeight();

    void setTriple(std::vector<Triple> tri);
    std::vector<Triple> getTriple();

    void setobtype(State obtype);
    State getobtype();

    void setObState(ObState obs);
    ObState getObState();

    void addMv(std::unique_ptr<Movement> &m);
    
    std::vector<Movement*> getMv();
    
    //bool ifMove();
    std::vector<Triple> nextPos();

    void clearMove();


};

#endif

