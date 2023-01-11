#ifndef MCYCLE_H
#define MCYCLE_H
#include "triple.h"
#include <vector>
#include "movementType.h"
#include "movement.h"
class MCycle: public Movement{
    //int speed;
    //int prog=0;

    //DIRECTION 
    //1: up
    //2: down
    //3: left
    //4: right
    int cycleSize;
    std::vector<std::vector<Triple>> triCycle;
    int cur = 0;
public:
    MCycle(int,std::vector<std::vector<Triple>>);
    std::vector<Triple> moveOb(std::vector<Triple> tri); 
};

#endif

