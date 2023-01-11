#include "mPC.h"

MPC::MPC():Movement(0,MovementType::PC){}

void MPC::setAction(Action act){ this->a =act; }


std::vector<Triple> MPC::moveOb(std::vector<Triple> tri){
    std::vector<Triple> tmp;
    if(a == Action::UP){
        for(auto i : tri){
            tmp.push_back(Triple(i.x-1,i.y,i.ch));
        }
    }else if(a == Action::DOWN){
        for(auto i : tri){
            tmp.push_back(Triple(i.x+1,i.y,i.ch));
        }
    }else if(a == Action::LEFT){
        for(auto i : tri){
            tmp.push_back(Triple(i.x,i.y-1,i.ch));
        }

    }else if(a == Action::RIGHT){
        for(auto i : tri){
            tmp.push_back(Triple(i.x,i.y+1,i.ch));
        }
        
    }else{
        tmp = tri;
    }
    return tmp;
}


