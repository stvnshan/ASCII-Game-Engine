#ifndef MOVEMENT_H
#define MOVEMENT_H
#include "triple.h"
#include <vector>
#include "movementType.h"
class Movement{
protected:
    int speed;
    int prog=0;
    MovementType mt;
    Movement(int speed, MovementType mt):speed{speed}, mt{mt} {}
public:
    bool ifMove();
    virtual std::vector<Triple> moveOb(std::vector<Triple> tri) = 0;
    MovementType getMType() { return mt; }
    int getSpeed(){ return speed; }
    void setSpeed(int s){ speed = s; }
    int getProg(){ return prog; }
    void setProg(int p){ prog=p; }
};



#endif


