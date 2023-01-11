#ifndef ENGINE_H
#define ENGINE_H
#include "model.h"
#include "state.h"
#include "object.h"
#include <string>
#include "gameState.h"
#include "timerObserver.h"
//#include "timerSubject.h"
//#include <iostream>
#include <vector>
#include "timer.h"
//#include "triple.h"
#include "movementType.h"
#include "movement.h"
#include "mPC.h"
#include "mStraight.h"
#include "mGravity.h"
#include "obstate.h"
#include "mCycle.h"
#include <stdlib.h>
#include <time.h>
#include <map>
#include <sstream>
using namespace std;



class Engine: public  Model, public TimerObserver{
    struct Cell{
        bool ifWall;
        std::vector<Object*> ob;
        Cell(bool ifWall = false);
    };
    
    int nxtID = 0;
    unique_ptr<Timer> clock;
    int sizeX = 22;
    int sizeY = 80;
    std::vector<std::vector<Cell>> grid;


   // std::map<int, std::unique_ptr<Object>> obmap;
    std::map<int, Object*> obmap;

    std::vector<std::unique_ptr<Object>> oblist;

    Object* player;
    int PlayerHP = 5;

    std::vector<Object*> needDelete;

    int sizeob = 0; 
    // 0 boarder is solid
    // 1 boarder is view
    int boarder = 0;
    int level = 1;
    int enemy = 0;
    int Score = 0;
    string state1 = "Status Bar 1";
    string state2 = "Status Bar 2";
    string state3 = "Status Bar 3";
    GameState gs = GameState::ONGOING;

    int levelup = 0;

    void refreshCell(int row, int col);

    bool firstime = true;
public:
    Engine();   

    void timeNotify() override;

    //void createPlayer()

    void moveEntity(Object *);

    void deleteEntity(Object *);
    
    //void addEntity(State, int, int);

    void collisionDection(Object *, std::vector<Triple> newPos);

    void bounceOff(Object *);

    void landing(Object *);

    void clearBoard();

    void go(int l=1);


    void generateE(int x, int y);

    void generateBall();


    bool checkEmptyCell(int x,int y);

    void generateFog();
};
#endif


