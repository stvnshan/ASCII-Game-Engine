#include "movement.h"

bool Movement::ifMove(){
    if(prog == speed){
        prog = 0;
        return true;
    }else{
        ++prog;
        return false;
    }
}


