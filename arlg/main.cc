#include "engine.h"
#include <string>
#include <iostream>
#include<sstream>
using namespace std;
int main(int argc, char *argv[]){
    Engine e;
    

    if(argc>1){
        string str = argv[1];
        stringstream ss(str);
        int x;
        ss>>x;
        e.go(x);
    }else{
        e.go();
    }
    
}

