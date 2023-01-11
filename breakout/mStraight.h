#ifndef MSTRAIGHT_H
#define MSTRAIGHT_H
#include "triple.h"
#include <vector>
#include "movementType.h"
#include "movement.h"
class MStraight: public Movement{
    //int speed;
    //int prog=0;

    //DIRECTION 
    //1: up
    //2: down
    //3: left
    //4: right
    int dir;
    
public:
    MStraight(int,int);
    std::vector<Triple> moveOb(std::vector<Triple> tri);
    int getDir();
    void setDir(int d);
   
};

#endif


