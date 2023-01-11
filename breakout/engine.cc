#include "engine.h"
#include "timerObserver.h"
#include "timer.h"  
#include <iostream>
#include <cstdlib>
#include "curseControl.h"
#include "curseView.h"
#include "color.h"


Engine::Cell::Cell(bool ifWall):ifWall{ifWall} {}



Engine::Engine(){
    srand(time(NULL));
    addView(std::move(std::make_unique<Curses>()));
    addController(std::move(std::make_unique<CurseKeyboard>()));

    // for(int i = 0; i < 80; ++i){
    //     std::vector<Cell> row;
    //     for(int j = 0; j < 23; ++j){
    //         if(i == 0 || i == 79){
    //             row.emplace_back(Cell(true));
            
    //         }else if(j==0 || j == 22){
    //             row.emplace_back(Cell(true));

    //         }else{
    //             row.emplace_back(Cell());
    //         }
    //     }
    //     grid.emplace_back(row);
    // }

    for(int i = 0; i <= 21; ++i){
        std::vector<Cell> row;
        for(int j = 0; j <= 79; ++j){
            if(i == 0 || i == 21){
                row.emplace_back(Cell(true));
            
            }else if(j==0 || j == 79){
                row.emplace_back(Cell(true));

            }else{
                row.emplace_back(Cell());
            }
        }
        grid.emplace_back(row);
    }

    //create timer
    clock = make_unique<Timer>();
    clock->attach(this);
}

void Engine::timeNotify(){


    if(gs == GameState::ONGOING){
        //std::cout<<"hello?";
        if(this->levelup == 1){
            go(level);
            this->levelup = 0;
        }
        Action a = getAction();
    
    // 
        if(a == Action::INVALID || a == Action::LEFT || a == Action::RIGHT || a==Action::INVALIDD){
            for(auto &i : player->getMv()){
                if(i->getMType() == MovementType::PC){
                    
                    MPC* mpc = static_cast<MPC*>(i);
                    mpc->setAction(a);
                
                }
            }
        }
        
        // for(auto &i : oblist){
        //     //std::cout<<"2"<<endl;
        //     Object* tmp = i.get();
        //     if(tmp->getobtype() == State::EXIT && enemy==0){
        //         vector<Triple> tmptri = tmp->getTriple();
        //         tmptri[0].ch = 'E';
        //         tmp->setTriple(tmptri);
        //     }
        //     moveEntity(tmp);
        // }

        for(int i=0; i<nxtID; ++i){
            Object* tmp;
            if(obmap.count(i)!=0){
                //tmp = obmap[i].get();
                tmp = obmap[i];
                

                moveEntity(tmp);

                for(int i = 0; i<needDelete.size();++i){
                    Object* tmp = needDelete[i];
                    if(tmp){
                        deleteEntity(tmp);
                    }
                }
                needDelete.clear();
            }
        }

       
        updateViews(1,state1);
        updateViews(2,state2);
        updateViews(3,state3);
        displayViews();
    }else if(gs == GameState::LOSE){
        clearBoard();
        updateViews(2,"Health: 0");
        displayViews();
        for(int i=1;i<20;++i){
            for(int j=5;j<67 ; j=j+9){
                updateViews(i,j,'Y',Color::RED);
                updateViews(i,j+1,'O',Color::RED);
                updateViews(i,j+2,'U',Color::RED);
                updateViews(i,j+3,' ',Color::RED);
                updateViews(i,j+4,'L',Color::RED);
                updateViews(i,j+5,'O',Color::RED);
                updateViews(i,j+6,'S',Color::RED);
                updateViews(i,j+7,'E',Color::RED);
                updateViews(i,j+8,'!',Color::RED);
            }
 
        }
        
        Action a = getAction();
        if(a != Action::INVALID){
            destroyViews();
            exit(0);
        }
    }else if(gs == GameState::WIN){
        clearBoard();
        string s = "CONGRATS YOU WIN!";
        stringstream ss(s);
        ss>>noskipws;
        char c;
        int i = 20;
        while(ss>>c){
            updateViews(5,i,c,Color::CYAN);
            i++;
        }
        s = "Thanks for playing the game!";
        stringstream ss2(s);
        ss2>>noskipws;
        i = 20;
        while(ss2>>c){
            updateViews(6,i,c,Color::CYAN);
            i++;
        }
        s = "This Game is Created By Steven Shan";
        stringstream ss3(s);
        ss3>>noskipws;
        i = 20;
        while(ss3>>c){
            updateViews(7,i,c,Color::CYAN);
            i++;
        }
        s = "NO Group Partner :((((";
        stringstream ss4(s);
        ss4>>noskipws;
        i = 20;
        while(ss4>>c){
            updateViews(8,i,c,Color::CYAN);
            i++;
        }
    
        displayViews();
    }

    
}


void Engine::moveEntity(Object *ob){
    
    std::vector<Triple> newPos = ob->nextPos();
    this->collisionDection(ob,newPos);

}


void Engine::collisionDection(Object *ob, std::vector<Triple> newPos){

    bool blocked = false;
    //modify by client
    for(auto i : newPos){
        if( this->grid[i.x][i.y].ifWall || this->grid[i.x][i.y].ob.size() != 0 ){
            
            if(ob->getobtype() == State::PLAYER){
                if(this->grid[i.x][i.y].ifWall){
                    blocked = true;
                }
                for(auto k : this->grid[i.x][i.y].ob){
                    if(k->getHeight() == ob->getHeight()){
                        if(k->getobtype() == State::BALL){
                            blocked = true;
                        }
                    }
                }

            }else if(ob->getobtype() == State::BALL){
                if(this->grid[i.x][i.y].ifWall){
                    std::vector<Movement*> tmp = ob->getMv();
                    
                    std::vector<MStraight*> updown;
                    std::vector<MStraight*> leftright;
                    for(auto &k : ob->getMv()){
                        MStraight* tmpStraight = static_cast<MStraight*>(k);
                        if(tmpStraight->getDir() == 1 || tmpStraight->getDir() == 2){
                            updown.emplace_back(tmpStraight);
                        }else{
                            leftright.emplace_back(tmpStraight);
                        }
                    }
                    
                    if(i.y==0 || i.y==79){
                        for(auto j:leftright){
                            if(j->getDir() == 3){
                                j->setDir(4);
                            }else if(j->getDir() == 4){
                                j->setDir(3);
                            }
                        }
                        
                    }else if(i.x==0){
                        for(auto j:updown){
                            if(j->getDir() == 1){
                                j->setDir(2);
                            }else if(j->getDir() == 2){
                                j->setDir(1);
                            }
                        }
                    
                    }else if(i.x==21){
                        needDelete.emplace_back(ob);
                        PlayerHP--;
                        state2 = "Heath "+std::to_string(PlayerHP);
                        if(PlayerHP == 0){
                            gs = GameState::LOSE;
                        }else{
                            generateBall();
                        }
                    }
                    blocked = true;
                    break;
                }else{
                    for(auto k : this->grid[i.x][i.y].ob){
                        if(ob->getHeight() == k->getHeight()){
                            if(k->getobtype() == State::PLAYER){
                                blocked = true;
                                int speed = ob->getMv()[0]->getSpeed();
                                ob->clearMove();
                                for(int j=0;j<16;j++){
                                    if(k->getTriple()[j].x==i.x &&k->getTriple()[j].y==i.y){
                                        if(j<=7){
                                            std::unique_ptr<Movement> mvtmp1 = std::make_unique<MStraight>(speed,1);
                                            ob->addMv(mvtmp1);
                                            
                                            if(j == 0 || j == 1){
                                                std::unique_ptr<Movement> mvtmp1 = std::make_unique<MStraight>(speed-2,3);        
                                                ob->addMv(mvtmp1);
                                                
                                            }else if(j == 2 || j == 3){
                                                std::unique_ptr<Movement> mvtmp1 = std::make_unique<MStraight>(speed-1,3);        
                                                ob->addMv(mvtmp1);
                                            }else if(j == 4 || j == 5){
                                                std::unique_ptr<Movement> mvtmp1 = std::make_unique<MStraight>(speed-1,3);        
                                                ob->addMv(mvtmp1);
                                            }else if(j == 6 || j == 7){
                                                std::unique_ptr<Movement> mvtmp1 = std::make_unique<MStraight>(speed,3);        
                                                ob->addMv(mvtmp1);
                                            }
                                        }else if(j>7){
                                            std::unique_ptr<Movement> mvtmp = std::make_unique<MStraight>(speed,1);
                                            ob->addMv(mvtmp);
                                            if(j == 14 || j == 15){
                                                std::unique_ptr<Movement> mvtmp1 = std::make_unique<MStraight>(speed,4);        
                                                ob->addMv(mvtmp1);
                                                
                                            }else if(j == 12 || j == 13){
                                                std::unique_ptr<Movement> mvtmp1 = std::make_unique<MStraight>(speed-1,4);        
                                                ob->addMv(mvtmp1);
                                            }else if(j == 10 || j == 11){
                                                std::unique_ptr<Movement> mvtmp1 = std::make_unique<MStraight>(speed-1,4);        
                                                ob->addMv(mvtmp1);
                                            }else if(j == 8 || j == 9){
                                                std::unique_ptr<Movement> mvtmp1 = std::make_unique<MStraight>(speed-2,4);        
                                                ob->addMv(mvtmp1);
                                            }
                                        }
                                    }
                                }
                            }else if(k->getobtype() == State::E){
                                needDelete.emplace_back(k);
                                Score++;
                                if(level==1){
                                    if(Score==10){
                                        level++;
                                        levelup=1;
                                    }
                                }else if(level == 2){
                                    if(Score==16){
                                        gs = GameState::WIN;
                                    }

                                }
                                if(level ==1){
                                    state3 = "Score "+std::to_string(Score)+"/10";
                                }else if(level ==2){
                                    state3 = "Score "+std::to_string(Score)+"/16";
                                }
                                
                                
                                for(auto &k: ob->getMv()){
                                    MStraight* tmp = static_cast<MStraight*>(k);
                                    if(tmp->getDir()==1){
                                        tmp->setDir(2);
                                    }else if(tmp->getDir()==2){
                                        tmp->setDir(1);
                                    }
                                }
                            }
                        }
                    }
                }
            }else if(ob->getobtype() == State::FOG){
                // if(this->grid[i.x][i.y].ifWall){
                //     bounceOff(ob);
                // }
            }
  
            
        }
    }

    
    //if not blocked by other cells
    if(!blocked && ob){
        
        //clear old cell
        for(auto i : ob->getTriple()){
            for(int j=0; j<this->grid[i.x][i.y].ob.size();j++){
                if(this->grid[i.x][i.y].ob[j] == ob){
                    grid[i.x][i.y].ob.erase(grid[i.x][i.y].ob.begin()+j);
                }
            }
            refreshCell(i.x,i.y);
        }
    
        //move to new cell
        for(auto i : newPos){
            //clean refresh cell
            bool update = true;
            //nothing above wall
            if(this->grid[i.x][i.y].ifWall) update=false;
            for(auto j : this->grid[i.x][i.y].ob){
                if(j->getHeight()>ob->getHeight()){
                    update = false;
                    break;
                }
            }
            if(update){
                //std::cout<<"1"<<endl;
                updateViews(i.x,i.y,i.ch);
            } 
            this->grid[i.x][i.y].ob.emplace_back(ob);
        }
        ob->setTriple(newPos);
    }
}

void Engine::deleteEntity(Object *ob){

    //delete it on the board
    for(auto i : ob->getTriple()){
        for(int j = 0; j< this->grid[i.x][i.y].ob.size();j++){
            if(this->grid[i.x][i.y].ob[j] == ob){
                grid[i.x][i.y].ob.erase(grid[i.x][i.y].ob.begin()+j);
            }
        }
        refreshCell(i.x,i.y);
    }

     //delete it in the obmap
    int id = ob->getID();
    if(obmap.count(id)!=0){
        obmap.erase(id);
    }

    
}

//refresh the cell
void Engine::refreshCell(int row,int col){
    if(this->grid[row][col].ifWall){
        if((row==0 && col==0) || (row==0 && col==79) || (row==21&&col==0)||(row==21&&col==79)){
            updateViews(row,col,'+');
        }else if(row ==0 || row==21){
            updateViews(row,col,'-');
        }else{
            updateViews(row,col,'|');
        }
    }else if(this->grid[row][col].ob.size()==0){
        updateViews(row,col,' ');
    }else{
        int height = this->grid[row][col].ob[0]->getHeight();
        std::vector<Triple> tri = this->grid[row][col].ob[0]->getTriple(); 
        for(int i=1;i<this->grid[row][col].ob.size();i++){
            if(this->grid[row][col].ob[i]->getHeight() > height){
                height = this->grid[row][col].ob[i]->getHeight();
                tri = this->grid[row][col].ob[i]->getTriple();
            }
        }
        char ch;
        for(auto i:tri){
            if(i.x == row && i.y == col){
                ch = i.ch;
                break;
            }
        }     
        if(ch=='P'){
            updateViews(row,col,ch,Color::RED);
        }else if(ch == 'O'){
            updateViews(row,col,ch,Color::CYAN);
        }else if(ch == 'D'){
            updateViews(row,col,ch,Color::GREEN);
        }else{
            updateViews(row,col,ch);
        }
        
    }
}

void Engine::bounceOff(Object *ob){
    for(auto &i: ob->getMv()){
        if(i->getMType() == MovementType::STRAIGHT){
            MStraight* tmp = static_cast<MStraight*>(i);
            if(tmp->getDir() == 1){
                tmp->setDir(2);
            }else if(tmp->getDir()==2){
                tmp->setDir(1);
            }else if(tmp->getDir()==3){
                tmp->setDir(4);
            }else if(tmp->getDir()==4){
                tmp->setDir(3);
            }
        }else if(i->getMType() == MovementType::GRAVITY){
            MGravity* tmp = static_cast<MGravity*>(i);
            if(tmp->getDir() == 1){
                tmp->setDir(2);
            }else if(tmp->getDir()==2){
                tmp->setDir(1);
            }else if(tmp->getDir()==3){
                tmp->setDir(4);
            }else if(tmp->getDir()==4){
                tmp->setDir(3);
            }
        }
    }
}

void Engine::landing(Object *ob){
    ob->clearMove();
}

void Engine::clearBoard(){
    this->obmap.clear();
    for(int i = 0; i <= 21; ++i){
        for(int j = 0; j <= 79; ++j){
            this->grid[i][j].ob.clear();
            refreshCell(i,j);
        }
    }
    nxtID = 0;
    this->enemy = 0;
    //player==nullptr;
    //
    this->oblist.clear();
    //
    displayViews();
}

void Engine::go(int l){
    Score = 0;
    level = l;
    state1 = "Level "+std::to_string(level)+"/2";
    state2 = "Heath "+std::to_string(PlayerHP);
    clearBoard();

    //create player
    std::vector<Triple> playerVec;
    int x,y;
    

    for(int i=0;i<16;i++){
        Triple t = Triple(19,30+i,'-');
        playerVec.push_back(t);
    }
    
    std::unique_ptr<Object> playerTmp = make_unique<Object>(nxtID, 0,playerVec,State::PLAYER,PlayerHP);
    std::unique_ptr<Movement> mvtmp = std::make_unique<MPC>();
    playerTmp->addMv(mvtmp);
    player = playerTmp.get();
    for(auto i:playerVec)   grid[i.x][i.y].ob.emplace_back(player);
    
    //oblist.emplace_back(playerTmp);


    
    //obmap.insert(std::pair<int, std::unique_ptr<Object>>(this->nxtID,std::move(playerTmp)));
    obmap.insert(std::pair<int, Object*>(this->nxtID,playerTmp.get()));
    oblist.emplace_back(std::move(playerTmp));
    nxtID++;


    for(auto i:playerVec) updateViews(i.x,i.y,i.ch);
    
    //state2 = "Health: "+std::to_string(PlayerHP);

    
    if(level == 1){
        state3 = "Score "+std::to_string(Score)+"/10";
        generateBall();
        generateE(3,10);
        generateE(5,10);
        generateE(7,10);
        generateE(9,10);
        
        generateE(3,20);
        generateE(5,20);
        generateE(7,20);
        generateE(9,20);

        generateE(3,50);
        generateE(5,50);
        generateE(7,50);
        generateE(9,50);

        generateE(3,60);
        generateE(5,60);
        generateE(7,60);
        generateE(9,60);

       
    }else if(level == 2){
        state3 = "Score "+std::to_string(Score)+"/16";
        generateBall();
        generateE(3,5);
        generateE(5,5);
        generateE(7,5);
        generateE(9,5);
        
        generateE(3,15);
        generateE(5,15);
        generateE(7,15);
        generateE(9,15);


        generateE(3,25);
        generateE(5,25);
        generateE(7,25);
        generateE(9,25);


        generateE(3,50);
        generateE(5,50);
        generateE(7,50);
        generateE(9,50);

        generateE(3,60);
        generateE(5,60);
        generateE(7,60);
        generateE(9,60);

        generateFog();


    }

    displayViews();


    if(firstime){
        firstime = false;
        clock->startTime();
    } 

}



bool Engine::checkEmptyCell(int x, int y){
    if(x<=0 || y<=0 || x>=22 || y>=80 || this->grid[x][y].ifWall || this->grid[x][y].ob.size()!=0){
        return false;
    }else{
        return true;
    }
}

void Engine::generateE(int x, int y){
    //srand(time(NULL));

    std::vector<Triple> eVec;
    
    
    eVec.push_back(Triple(x,y,'='));
    eVec.push_back(Triple(x,y+1,'='));
    eVec.push_back(Triple(x,y+2,'='));
    eVec.push_back(Triple(x,y+3,'='));
    eVec.push_back(Triple(x,y+4,'='));
    eVec.push_back(Triple(x,y+5,'='));

    std::unique_ptr<Object> eTmp = std::make_unique<Object>(nxtID,0,eVec,State::E,1);

    Object* e = eTmp.get();
    for(auto i: eVec) grid[i.x][i.y].ob.emplace_back(e);
    //grid[x][y].ob.emplace_back(Popup);
    //oblist.emplace_back(playerTmp);


    //obmap.insert(std::pair<int,std::unique_ptr<Object>>(nxtID, std::move(exitTmp)));
    obmap.insert(std::pair<int,Object*>(nxtID, eTmp.get()));
    oblist.emplace_back(std::move(eTmp));
    nxtID++;


    for(auto i:e->getTriple()){
        refreshCell(i.x,i.y);
    }
    

}

void Engine::generateBall(){
    std::vector<Triple> ballVec;
    ballVec.push_back(Triple(10,40,'O'));
    std::unique_ptr<Object> ballTmp = std::make_unique<Object>(nxtID,0,ballVec,State::BALL,1);

    
    std::unique_ptr<Movement> mvtmp1 = std::make_unique<MStraight>(3,2);
    std::unique_ptr<Movement> mvtmp2 = std::make_unique<MStraight>(5,3);
    ballTmp->addMv(mvtmp1);
    ballTmp->addMv(mvtmp2);

    Object* ball = ballTmp.get();
    for(auto i: ballVec) grid[i.x][i.y].ob.emplace_back(ball);
    //grid[x][y].ob.emplace_back(Popup);
    //oblist.emplace_back(playerTmp);


    //obmap.insert(std::pair<int,std::unique_ptr<Object>>(nxtID, std::move(exitTmp)));
    obmap.insert(std::pair<int,Object*>(nxtID, ballTmp.get()));
    oblist.emplace_back(std::move(ballTmp));
    nxtID++;
    for(auto i:ball->getTriple()){
        refreshCell(i.x,i.y);
    }

}

void Engine::generateFog(){
    std::vector<Triple> fogVec1;
    std::vector<Triple> fogVec2;
    std::vector<Triple> fogVec3;
    std::vector<Triple> fogVec4;
    std::vector<Triple> fogVec5;

    for(int i = 1;i<=10;++i){
        for(int j=1;j<=39;j++){
            fogVec1.push_back(Triple(i,j,'F'));
        }
    }
    for(int i = 1;i<=10;++i){
        for(int j=40;j<=78;j++){
            fogVec2.push_back(Triple(i,j,'O'));
        }
    }
    for(int i = 11;i<=20;++i){
        for(int j=1;j<=39;j++){
            fogVec3.push_back(Triple(i,j,'G'));
        }
    }
    for(int i = 11;i<=20;++i){
        for(int j=40;j<=78;j++){
            fogVec4.push_back(Triple(i,j,'!'));
        }
    }
    for(int i = 1;i<=10;++i){
        for(int j=1;j<=78;j++){
            fogVec5.push_back(Triple(i,j,'X'));
        }
    }

    std::vector<std::vector<Triple>> triCycle;
    triCycle.emplace_back(fogVec5);
    triCycle.emplace_back(fogVec1);
    triCycle.emplace_back(fogVec2);
    triCycle.emplace_back(fogVec3);
    triCycle.emplace_back(fogVec4);
    
    std::vector<Triple> fogVec;
    std::unique_ptr<Object> fogTmp = std::make_unique<Object>(nxtID,10,fogVec,State::FOG,1);

    
    std::unique_ptr<Movement> mvtmp1 = std::make_unique<MCycle>(20,triCycle);

    fogTmp->addMv(mvtmp1);
  

    Object* fog = fogTmp.get();
    for(auto i: fogVec) grid[i.x][i.y].ob.emplace_back(fog);
    //grid[x][y].ob.emplace_back(Popup);
    //oblist.emplace_back(playerTmp);


    //obmap.insert(std::pair<int,std::unique_ptr<Object>>(nxtID, std::move(exitTmp)));
    obmap.insert(std::pair<int,Object*>(nxtID, fogTmp.get()));
    oblist.emplace_back(std::move(fogTmp));
    nxtID++;
    for(auto i:fog->getTriple()){
        refreshCell(i.x,i.y);
    }
}



