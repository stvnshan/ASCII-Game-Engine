#include "mCycle.h"

MCycle::MCycle(int speed,std::vector<std::vector<Triple>> triCycle): Movement(speed,MovementType::CYCLE),triCycle{std::move(triCycle)}{
    cycleSize = triCycle.size();
}

std::vector<Triple> MCycle::moveOb(std::vector<Triple>){
    if(cur+1<cycleSize){
        cur++;
        return triCycle[cur];
    }else{
        cur = 0;
        return triCycle[cur];
    }
}



