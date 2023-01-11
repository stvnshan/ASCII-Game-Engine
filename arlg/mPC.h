#ifndef MPC_H
#define MPC_H
#include "triple.h"
#include <vector>
#include "movementType.h"
#include "movement.h"
#include "action.h"

class MPC: public Movement{
    Action a = Action::RIGHT;
    //Action a = Action::INVALID;
public:
    MPC();
    void setAction(Action);
    std::vector<Triple> moveOb(std::vector<Triple> tri);
};


#endif


