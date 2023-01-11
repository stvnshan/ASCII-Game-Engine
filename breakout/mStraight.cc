#include "mStraight.h"

MStraight::MStraight(int speed,int dir): Movement(speed,MovementType::STRAIGHT),dir{dir}{}

std::vector<Triple> MStraight::moveOb(std::vector<Triple> tri){
    std::vector<Triple> tmp;
    if(dir == 1){
        for(auto i : tri){
            tmp.push_back(Triple(i.x-1,i.y,i.ch));
        }
    }else if(dir == 2){
        for(auto i : tri){
            tmp.push_back(Triple(i.x+1,i.y,i.ch));
        }
    }else if(dir == 3){
        for(auto i : tri){
            tmp.push_back(Triple(i.x,i.y-1,i.ch));
        }

    }else if(dir == 4){
        for(auto i : tri){
            tmp.push_back(Triple(i.x,i.y+1,i.ch));
        }
        
    }
    
    return tmp;
}

int MStraight::getDir(){
    return dir;
}
void MStraight::setDir(int d){
    dir = d;
}



