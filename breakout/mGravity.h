#ifndef MGRAVITY_H
#define MGRAVITY_H
#include "triple.h"
#include <vector>
#include "movementType.h"
#include "movement.h"
class MGravity: public Movement{
    //int speed;
    //int prog=0;

    //DIRECTION 
    //1: up
    //2: down
    //3: left
    //4: right
    int dir;
public:
    MGravity(int);
    std::vector<Triple> moveOb(std::vector<Triple> tri);
    void setDir(int d);
    int getDir();

};

#endif

