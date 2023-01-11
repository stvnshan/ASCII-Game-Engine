#include "object.h"

Object::Object(int ID, int height, std::vector<Triple> tri, State obtype, int hp, int changeMv, ObState obstate):ID{ID}, height{height}, tri{std::move(tri)}, obtype{obtype},hp{hp},changeMv{changeMv}, obstate{obstate}{}

void Object::setHeight(int height){ this->height = height; }
int Object::getHeight(){ return height; }

void Object::setTriple(std::vector<Triple> trii){ this->tri = trii; }
std::vector<Triple> Object::getTriple(){
    return this->tri;
}

void Object::setChangeMv(int changeMv){this->changeMv = changeMv;}
int Object::getChangeMv(){return this->changeMv;}

void Object::setChangeProg(int changeProg){this->changeProg = changeProg;}
int Object::getchangeProg(){return this->changeProg;}

void Object::setobtype(State obtype){ this->obtype = obtype; }
State Object::getobtype(){ return obtype; }

void Object::setHp(int Hp){this->hp = Hp;}
int Object::getHp(){return this->hp;}

void Object::setObState(ObState obs){ this->obstate = obs; }
ObState Object::getObState(){ return this->obstate; }

void Object::addMv(std::unique_ptr<Movement> &m){ 
    mv.emplace_back(std::move(m)); 
}

std::vector<Movement*> Object::getMv(){ 
    std::vector<Movement*> myMv;
    for(auto &i : this->mv){
        Movement *tmp = i.get();
        myMv.emplace_back(tmp);
    }
    return myMv;
}


// bool Object::ifMove(){
//     bool ifmove = false;
//     for(auto i : this->mv){
//         if(i->ifMove()){
//             ifmove = true;
//         }
//     }
//     return ifmove;
// }

int Object::getID(){ return ID; }

std::vector<Triple> Object::nextPos(){
    std::vector<Triple> copy = this->tri;
    for(auto &i : this->mv){
        if(i->ifMove()){
            copy = i->moveOb(copy);
        }
    }
    return copy;
}

void Object::clearMove(){
    this->mv.clear();
}



