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
        if(a == Action::INVALID || a == Action::UP || a == Action::DOWN || a == Action::LEFT || a == Action::RIGHT || a==Action::INVALIDD){
            for(auto &i : player->getMv()){
                if(i->getMType() == MovementType::PC){
                    
                    MPC* mpc = static_cast<MPC*>(i);
                    mpc->setAction(a);
                
                }
            }
        }else if(a == Action::W){
            generateProj(1, 'O',player);
        }else if(a == Action::A){
            generateProj(3, 'O',player);
        }else if(a == Action::S){
            generateProj(2, 'O',player);
        }else if(a == Action::D){
            generateProj(4, 'O',player);
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
                if(tmp->getobtype() == State::EXIT && enemy==0){
                    vector<Triple> tmptri = tmp->getTriple();
                    tmptri[0].ch = 'E';
                    tmp->setTriple(tmptri);
                }else if(tmp->getobtype() == State::POPUPS && tmp->getchangeProg()%6==0 &&tmp->getObState() == ObState::VULNERABLE){
                    generateProj(1, 'D',tmp);
                    generateProj(2, 'D',tmp);
                    generateProj(3, 'D',tmp);
                    generateProj(4, 'D',tmp);
                }else if(tmp->getobtype() == State::STALKER){
                    tmp->clearMove();
                    if(tmp->getChangeMv() == tmp->getchangeProg()){
                        tmp->setChangeProg(0);
                        if(tmp->getTriple()[0].x > player->getTriple()[0].x){
                            std::unique_ptr<Movement> mvtmp = std::make_unique<MStraight>(0,1);
                            tmp->addMv(mvtmp);
                        }else if(tmp->getTriple()[0].x < player->getTriple()[0].x){
                            std::unique_ptr<Movement> mvtmp = std::make_unique<MStraight>(0,2);
                            tmp->addMv(mvtmp);
                        }
                        if(tmp->getTriple()[0].y > player->getTriple()[0].y){
                            std::unique_ptr<Movement> mvtmp = std::make_unique<MStraight>(0,3);
                            tmp->addMv(mvtmp);
                        }else if(tmp->getTriple()[0].y < player->getTriple()[0].y){
                            std::unique_ptr<Movement> mvtmp = std::make_unique<MStraight>(0,4);
                            tmp->addMv(mvtmp);
                        }
                    }else{
                        tmp->setChangeProg(tmp->getchangeProg()+1);
                    }
                }else if(tmp->getobtype() == State::BOSS){
                    state3 = "BOSS HP: "+std::to_string(tmp->getHp());
                    if(tmp->getObState() == ObState::PEACE && tmp->getHp() <= 0){
                        tmp->setHp(30);
                        tmp->setObState(ObState::RAGE);
                        std::vector<Triple> tri = tmp->getTriple();
                        tri[7].ch = 'V';
                        tri[8].ch = 'V';
                        tri[9].ch = 'V';
                        tri[10].ch = 'V';
                        tri[11].ch = 'V';
                        tri[28].ch = ',';
                        tri[34].ch = ',';
                        tri[41].ch = ' ';
                        tri[42].ch = '.';
                        tri[43].ch = '-';
                        tri[44].ch = '-';
                        tri[45].ch = '-';
                        tri[46].ch = '.';
                        tri[47].ch = ' ';
                        tmp->setTriple(tri);
                        Movement* tmpm = (tmp->getMv())[0];
                        tmpm->setSpeed(1);
                        tmpm->setProg(0);
                        tmpm = (tmp->getMv())[1];
                        tmpm->setSpeed(1);
                        tmpm->setProg(0);


                        //for(auto i: tri) refreshCell(i.x,i.y);
                    }else if(tmp->getObState() == ObState::RAGE && tmp->getHp() <= 0){
                        gs=GameState::WIN;
                        //clearBoard();
                    }
                }

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
    if(ob->getobtype() == State::WALKER){
        if(ob->getchangeProg()==ob->getChangeMv()){
            ob->setChangeProg(0);
            MStraight* tmpMv = static_cast<MStraight*>(ob->getMv()[0]);
            int dir;
            do{
                dir = 1+rand()%4;
            }while(dir==tmpMv->getDir());
            tmpMv->setDir(dir);
        }else{
            ob->setChangeProg(ob->getchangeProg()+1);
        }
    }else if(ob->getobtype() == State::POPUPS ){
        if(ob->getchangeProg() == ob->getChangeMv() ){
            if(ob->getObState() == ObState::INVULNERABLE){
                ob->setChangeProg(0);
                ob->setObState(ObState::VULNERABLE);
                ob->setChangeMv(120+rand()%120);
            }else if(ob->getObState() == ObState::VULNERABLE){
                ob->setChangeProg(0);
                ob->setObState(ObState::INVULNERABLE);
                ob->setChangeMv(120+rand()%120);
            }
        }else{
            ob->setChangeProg(ob->getchangeProg()+1);
        }

    }else if(ob->getobtype() == State::BOSS && ob->getObState() == ObState::RAGE){
        if(ob->getchangeProg() == ob->getChangeMv()){
            ob->setChangeProg(0);
            generateWalker();
            generateWalker();
            generateStalker();
            //generateStalker();
            generateSnake();
            generateSnake();
            generatePopup();
            //generatePopup();
        }else{
            ob->setChangeProg(ob->getchangeProg()+1);
        }
    }
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
                    if(k->getobtype()==State::WALKER){
                        if(ob->getHp()<=1){
                            ob->setHp(ob->getHp()-1);
                            PlayerHP--;
                            updateViews(2,"Health: 0");
                            gs = GameState::LOSE;
                            
                        }else{
                            ob->setHp(ob->getHp()-1);
                            PlayerHP--;
                            state2 = "Health: "+std::to_string(ob->getHp());
                        }
                        this->bounceOff(k);
                        blocked = true;
                    }else if(k->getobtype()==State::EXIT){
                        if(k->getTriple()[0].ch == 'E'){
                            this->level++;
                            this->levelup = 1;
                        }
                    }else if(k->getobtype()==State::FIRE){
                        if(ob->getHp()<=1){
                            ob->setHp(ob->getHp()-1);
                            PlayerHP--;
                            updateViews(2,"Health: 0");
                            gs = GameState::LOSE;
                        }else{
                            ob->setHp(ob->getHp()-1);
                            PlayerHP--;
                            state2 = "Health: "+std::to_string(ob->getHp());
                            
                        }
                        blocked = true;
                    }else if(k->getobtype()==State::HP){
                        needDelete.emplace_back(k);
                        ob->setHp(ob->getHp()+1);
                        PlayerHP++;
                        state2 = "Health: "+std::to_string(ob->getHp());
                        
                        
                    }else if(k->getobtype() == State::PROJ){
                        
                        needDelete.emplace_back(k);
                        if(ob->getHp()<=1){
                            updateViews(2,"Health: "+std::to_string(0));
                            gs = GameState::LOSE;
                        }else{
                            ob->setHp(ob->getHp()-1);
                            PlayerHP--;
                            state2 = "Health: "+std::to_string(ob->getHp());
                            
                        }
                        
                    }else if(k->getobtype() == State::POPUPS || k->getobtype() == State::STALKER || k->getobtype() == State::SNAKES ||k->getobtype() == State::BOSS){
                        if(ob->getHp()<=1){
                            updateViews(2,"Health: "+std::to_string(0));
                            gs = GameState::LOSE;
                        }else{
                            ob->setHp(ob->getHp()-1);
                            PlayerHP--;
                            state2 = "Health: "+std::to_string(ob->getHp());
                            
                        }
                        blocked = true;
                    }
                }

            }else if(ob->getobtype()== State::PROJ){
                if(this->grid[i.x][i.y].ifWall){
                    needDelete.emplace_back(ob);
                    blocked = true;
                    //break;
                }else{
                    for(auto k : this->grid[i.x][i.y].ob){
                        if(k->getobtype()==State::WALKER){
                            
                            if(k->getHp()<=1){
                                enemy--;
                                needDelete.emplace_back(k);
                            }else{
                                k->setHp(k->getHp()-1);
                            }
                            needDelete.emplace_back(ob);
                            //this->bounceOff(k);
                            blocked = true;
                            //break;
                        }else if(k->getobtype()==State::EXIT){
            
                        }else if(k->getobtype()==State::FIRE){
                            
                            if(rand()%2){
                                k->setobtype(State::HP);
                                std::vector<Triple> hp = k->getTriple();
                                hp[0].ch = 'h';
                                k->setTriple(hp);
                                refreshCell(hp[0].x,hp[0].y);
                            }else{
                                needDelete.emplace_back(k);
                            }
                            needDelete.emplace_back(ob);
                            blocked = true;
                            //break;
                        }else if(k->getobtype()==State::PROJ){
                            needDelete.emplace_back(ob);
                            needDelete.emplace_back(k);
                            blocked = true;
                            //break;
                        }else if(k->getobtype() == State::PLAYER){
                            needDelete.emplace_back(ob);
                            if(k->getHp()<=1){
                                updateViews(2,"Health: "+std::to_string(0));
                                gs = GameState::LOSE;
                            }else{
                                k->setHp(k->getHp()-1);
                                PlayerHP--;
                                state2 = "Health: "+std::to_string(k->getHp());
                                
                            }
                            blocked = true;
                            //break;
                        }else if(k->getobtype() == State::POPUPS){
                            needDelete.emplace_back(ob);
                            if(k->getObState() == ObState::VULNERABLE){
                                if(k->getHp()<=1){
                                    enemy--;
                                    needDelete.emplace_back(k);
                                }else{
                                    k->setHp(k->getHp()-1);
                                }
                                //this->bounceOff(k);
                                
                            }
                            blocked = true;
                            //break;
                        }else if(k->getobtype() == State::STALKER){
                            needDelete.emplace_back(ob);
                            if(k->getHp()<=1){
                                if(rand()%3==0){
                                    k->setobtype(State::WALKER);
                                    k->setHp(3);
                                    std::vector<Triple> tmp = k->getTriple();
                                    tmp[0].ch = 'a';
                                    tmp[1].ch = 'a';
                                    tmp[2].ch = 'a';
                                    tmp[3].ch = 'a';
                                    //for(auto i:tmp) i.ch = 'a';
                                    k->setTriple(tmp);
                                    k->clearMove();
                                    std::unique_ptr<Movement> mvtmp = std::make_unique<MStraight>(2,1+rand()%4);
                                    k->addMv(mvtmp);
                                    //for(auto i)
                                }else{
                                    enemy--;
                                    needDelete.emplace_back(k);
                                }
                            }else{
                                k->setHp(k->getHp()-1);
                            }
                            //this->bounceOff(k);
                                
                            
                            blocked = true;
                        }else if(k->getobtype()==State::SNAKES){
                            needDelete.emplace_back(ob);
                            for(auto i: k->getTriple()){
                                if(i.x == newPos[0].x && i.y == newPos[0].y && i.ch != 'Z'){
                                    if(k->getHp()<=1){
                                        enemy--;
                                        needDelete.emplace_back(k);
                                    }else{
                                        k->setHp(k->getHp()-1);
                                    }
                                }
                            }
                            
                            // needDelete.emplace_back(ob);
                            // //this->bounceOff(k);
                            blocked = true;
                            // //break;
                        }else if(k->getobtype() == State::BOSS){
                            needDelete.emplace_back(ob);
                                
                            k->setHp(k->getHp()-1);
                            
                            blocked = true;
                        }
                    }
                } 
  
            }else if(ob->getobtype() == State::WALKER){
                if(this->grid[i.x][i.y].ifWall){
                    bounceOff(ob);
                    blocked = true;
                }else{
                    for(auto k : this->grid[i.x][i.y].ob){
                        if(k->getobtype()==State::PLAYER){
                            if(k->getHp()<=1){
                                updateViews(2,"Health: "+std::to_string(0));
                                gs = GameState::LOSE;
                            }else{
                                k->setHp(k->getHp()-1);
                                PlayerHP--;
                                state2 = "Health: "+std::to_string(k->getHp());
                                
                            }
                            this->bounceOff(ob);
                            blocked = true;
                        }else if(k->getobtype() == State::PROJ){
                            
                            needDelete.emplace_back(k);
                            if(ob->getHp()<=1){
                                this->enemy--;
                                needDelete.emplace_back(ob);
                                blocked = true;
                            }else{
                                ob->setHp(ob->getHp()-1);
                                blocked = true;
                            }
                            
                        }else if(k->getobtype()==State::EXIT){
                    
                        }else if(k->getobtype()==State::FIRE){
                            if(ob->getHp()<=1){
                                this->enemy--;
                                needDelete.emplace_back(ob);
                            }else{
                                ob->setHp(ob->getHp()-1);
                            }
                            blocked = true;
                        }else if(k->getobtype()==State::HP){
                        }
                    }
                }
            }else if(ob->getobtype() == State::SNAKES){
                if(this->grid[i.x][i.y].ifWall){
                    std::vector<Triple> tri = ob->getTriple();
                    if(tri[0].ch == 'Z'){
                        tri[3].ch = 'Z';
                        tri[0].ch = 'z';
                    }else{
                        tri[3].ch = 'z';
                        tri[0].ch = 'Z';
                    }
                    ob->setTriple(tri);
                    bounceOff(ob);
            
                    blocked = true;
                }else{
                    for(auto k : this->grid[i.x][i.y].ob){
                        if(k->getobtype()==State::PLAYER){
                            if(k->getHp()<=1){
                                updateViews(2,"Health: "+std::to_string(0));
                                gs = GameState::LOSE;
                            }else{
                                k->setHp(k->getHp()-1);
                                PlayerHP--;
                                state2 = "Health: "+std::to_string(k->getHp());
                                
                            }
                            //this->bounceOff(ob);
                            blocked = true;
                        }else if(k->getobtype() == State::PROJ){
                            needDelete.emplace_back(k);
                        }else if(k->getobtype() == State::EXIT){
                        }else if(k->getobtype() == State::FIRE){
                            if(ob->getHp()<=1){
                                enemy--;
                                needDelete.emplace_back(ob);
                            }else{
                                ob->setHp(ob->getHp()-1);
                                std::vector<Triple> tri = ob->getTriple();
                                if(tri[0].ch == 'Z'){
                                    tri[3].ch = 'Z';
                                    tri[0].ch = 'z';
                                }else{
                                    tri[3].ch = 'z';
                                    tri[0].ch = 'Z';
                                }
                                ob->setTriple(tri);
                                bounceOff(ob);
                                blocked = true;
                            }
                            blocked = true;
                        }else if(k->getobtype()==State::HP){
                        }
                    }
                }
            }else if(ob->getobtype() == State::STALKER){
                if(this->grid[i.x][i.y].ifWall){
                    //bounceOff(ob);
                    blocked = true;
                }else{
                    for(auto k : this->grid[i.x][i.y].ob){
                        if(k->getobtype()==State::PLAYER){
                            if(k->getHp()<=1){
                                updateViews(2,"Health: "+std::to_string(0));
                                gs = GameState::LOSE;
                            }else{
                                k->setHp(k->getHp()-1);
                                PlayerHP--;
                                state2 = "Health: "+std::to_string(k->getHp());
                                
                            }
                            //this->bounceOff(ob);
                            blocked = true;
                        }else if(k->getobtype() == State::PROJ){
                            
                            needDelete.emplace_back(k);
                            if(ob->getHp()<=1){
                                if(rand()%3==0){
                                    ob->setobtype(State::WALKER);
                                    ob->setHp(3);
                                    std::vector<Triple> tmp = ob->getTriple();
                                    tmp[0].ch = 'a';
                                    tmp[1].ch = 'a';
                                    tmp[2].ch = 'a';
                                    tmp[3].ch = 'a';
                                    ob->setTriple(tmp);
                                    ob->clearMove();
                                    std::unique_ptr<Movement> mvtmp = std::make_unique<MStraight>(2,1+rand()%4);
                                    ob->addMv(mvtmp);

                                }else{
                                    enemy--;
                                    needDelete.emplace_back(ob);
                                }
                            }else{
                                ob->setHp(ob->getHp()-1);
                                blocked = true;
                            }
                            
                        }else if(k->getobtype()==State::EXIT){
                    
                        }else if(k->getobtype()==State::FIRE){
                            if(ob->getHp()<=1){
                                if(rand()%3==0){
                                    ob->setobtype(State::WALKER);
                                    ob->setHp(3);
                                    std::vector<Triple> tmp = ob->getTriple();
                                    tmp[0].ch = 'a';
                                    tmp[1].ch = 'a';
                                    tmp[2].ch = 'a';
                                    tmp[3].ch = 'a';
                                    //for(auto i:tmp) i.ch = 'a';
                                    ob->setTriple(tmp);
                                    ob->clearMove();
                                    std::unique_ptr<Movement> mvtmp = std::make_unique<MStraight>(2,1+rand()%4);
                                    ob->addMv(mvtmp);

                                }else{
                                    enemy--;
                                    needDelete.emplace_back(ob);
                                }
                            }else{
                                ob->setHp(ob->getHp()-1);
                            }
                            blocked = true;
                        }else if(k->getobtype()==State::HP){
                        }
                    }
                }


            }else if(ob->getobtype() == State::BOSS){
                if(this->grid[i.x][i.y].ifWall){
                    //bounceOff(ob);
                    std::vector<Movement*> tmp = ob->getMv();
                    MStraight* updown = static_cast<MStraight*>(tmp[0]);
                    MStraight* leftright = static_cast<MStraight*>(tmp[1]);
                    if(i.y==0 || i.y==79){
                        if(leftright->getDir() == 3){
                            leftright->setDir(4);
                        }else if(leftright->getDir() == 4){
                            leftright->setDir(3);
                        }
                    }else if(i.x==0 || i.x==21){
                        if(updown->getDir() == 1){
                            updown->setDir(2);
                        }else if(updown->getDir() == 2){
                            updown->setDir(1);
                        }
                    }
                    blocked = true;
                    break;
                }else{
                    for(auto k : this->grid[i.x][i.y].ob){
                        if(k->getobtype()==State::PLAYER){
                            if(k->getHp()<=1){
                                updateViews(2,"Health: "+std::to_string(0));
                                gs = GameState::LOSE;
                            }else{
                                k->setHp(k->getHp()-1);
                                PlayerHP--;
                                state2 = "Health: "+std::to_string(k->getHp());
                                
                            }
                            //this->bounceOff(ob);
                            blocked = true;
                        }else if(k->getobtype() == State::PROJ){
                            
                            needDelete.emplace_back(k);
                            
                            ob->setHp(ob->getHp()-1);
                            blocked = true;
                            
                            
                        }else if(k->getobtype()==State::EXIT){
                    
                        }else if(k->getobtype()==State::FIRE){
                            
                        }else if(k->getobtype()==State::HP){
                        }
                    }

                }


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
    level = l;
    state1 = "Level "+std::to_string(level)+"/6";
    clearBoard();

    //create player
    std::vector<Triple> playerVec;
    int x,y;
    do{
        x = rand() % sizeX;
        y = rand() % sizeY;
    }while(this->grid[x][y].ifWall || this->grid[x][y].ob.size()!=0);
    Triple t = Triple(x,y,'p');
    playerVec.push_back(t);
    std::unique_ptr<Object> playerTmp = make_unique<Object>(nxtID, 0,playerVec,State::PLAYER,PlayerHP);
    std::unique_ptr<Movement> mvtmp = std::make_unique<MPC>();
    playerTmp->addMv(mvtmp);
    player = playerTmp.get();
    grid[x][y].ob.emplace_back(player);
    //oblist.emplace_back(playerTmp);


    
    //obmap.insert(std::pair<int, std::unique_ptr<Object>>(this->nxtID,std::move(playerTmp)));
    obmap.insert(std::pair<int, Object*>(this->nxtID,playerTmp.get()));
    oblist.emplace_back(std::move(playerTmp));
    nxtID++;


    updateViews(x,y,State::PLAYER);
    
    state2 = "Health: "+std::to_string(PlayerHP);

    
    if(level == 1){
        int firenum = 10+rand()%5;
        for(int i = 0; i<=firenum;i++){
            generateFire();
        }
        displayViews();
        //int walderNum = 5+rand()%5;
        for(int i = 0; i<=4 ; i++){
            generateWalker();
            this->enemy++;
        }
        displayViews();
        generateExit();
        //displayViews();
        
    }else if(level == 2){

        int firenum = rand()%6;
        for(int i = 0; i<=firenum;i++){
            generateFire();
        }
        for(int i = 0; i<=2 ; i++){
            generateWalker();
            this->enemy++;
        }

        for(int i = 0; i<=3 ; i++){
            generateStalker();
            this->enemy++;
        }
        //displayViews();
        generateExit();

    }else if(level == 3){

        int firenum = rand()%5;
        for(int i = 0; i<=firenum;i++){
            generateFire();
        }
        for(int i = 0; i<=4 ; i++){
            generateWalker();
            this->enemy++;
        }
        for(int i=0; i<=1; i++){
            generatePopup();
            this->enemy++;
        }
        generateExit();
        
    }else if(level == 4){
        int firenum = rand()%5;
        for(int i = 0; i<=firenum;i++){
            generateFire();
        }
        for(int i = 0; i<=2 ; i++){
            generateStalker();
            this->enemy++;
        }
        for(int i=0; i<=5; i++){
            generateSnake();
            this->enemy++;
        }
        generateExit();


    }else if(level == 5){
        int firenum = rand()%4;
        for(int i = 0; i<=firenum;i++){
            generateFire();
        }
        for(int i = 0; i<=2 ; i++){
            generateWalker();
            this->enemy++;
        }
        for(int i=0; i<=1; i++){
            generateStalker();
            this->enemy++;
        }
        for(int i=0; i<=1; i++){
            generatePopup();
            this->enemy++;
        }
        for(int i=0; i<=2; i++){
            generateSnake();
            this->enemy++;
        }
        generateExit();

    }else if(level == 6){
        generateBoss();
        state3 = "BOSS HP: 20";
    }else if(level == 7){
        updateViews(3,"You Win!");
    }
    displayViews();


    if(firstime){
        firstime = false;
        clock->startTime();
    } 
    //clock->startTime();
}

void Engine::generateFire(){
    //srand(time(NULL));
    std::vector<Triple> fireVec;
    int x,y;
    do{
        x = 1 + rand() % (sizeX-1);
        y = 1 + rand() % (sizeY-1);
    }while(this->grid[x][y].ifWall || this->grid[x][y].ob.size()!=0);
    Triple t = Triple(x,y,'x');
    if(rand()%2){
        t = Triple(x,y,'x');
    }else{
        t = Triple(x,y,'X');
    }
    fireVec.push_back(t);
    std::unique_ptr<Object> fireTmp = std::make_unique<Object>(nxtID,0,fireVec,State::FIRE,1);
    Object* fire = fireTmp.get();
    grid[x][y].ob.emplace_back(fire);
    //oblist.emplace_back(playerTmp);


    //obmap.insert(std::pair<int,std::unique_ptr<Object>>(nxtID, std::move(fireTmp)));
    obmap.insert(std::pair<int,Object*>(nxtID, fireTmp.get()));
    oblist.emplace_back(std::move(fireTmp));
    nxtID++;
    refreshCell(x,y);
}

void Engine::generateProj(int dir, char ch, Object* ob){
    //srand(time(NULL));
    std::vector<Triple> projVec;
    projVec = ob->getTriple();
    if(dir == 1){
        projVec[0].x = projVec[0].x-1;
    }else if(dir == 2){
        projVec[0].x = projVec[0].x+1;
    }else if(dir == 3){
        projVec[0].y = projVec[0].y-1;
    }else if(dir == 4){
        projVec[0].y = projVec[0].y+1;
    }
    if(!grid[projVec[0].x][projVec[0].y].ifWall){
        projVec[0].ch = ch;
    
        std::unique_ptr<Object> projTmp = std::make_unique<Object>(nxtID, 0,projVec,State::PROJ,0);
        std::unique_ptr<Movement> mvTmp = std::make_unique<MStraight>(0,dir);

        projTmp->addMv(mvTmp);

        Object* proj = projTmp.get();


        for(auto i : projVec){
            grid[i.x][i.y].ob.emplace_back(proj);
        }
        
        //obmap.insert(std::pair<int,std::unique_ptr<Object>>(nxtID, std::move(projTmp)));
        obmap.insert(std::pair<int,Object*>(nxtID, projTmp.get()));
        oblist.emplace_back(std::move(projTmp));
        nxtID++;

        for(auto i:proj->getTriple()){
            refreshCell(i.x,i.y);
        }
    }
    
}

void Engine::generateWalker(){
    //srand(time(NULL));

    std::vector<Triple> walkerVec;
    int x,y;
    bool ifPlace = true;
    do{
        ifPlace = true;
        x = rand() % sizeX;
        y = rand() % sizeY;
        if(!checkEmptyCell(x,y)) ifPlace = false;
        if(!checkEmptyCell(x+1,y)) ifPlace = false; 
        if(!checkEmptyCell(x,y+1)) ifPlace = false; 
        if(!checkEmptyCell(x+1,y+1)) ifPlace = false;  
    }while(!ifPlace);
    
    walkerVec.push_back(Triple(x,y,'a'));
    walkerVec.push_back(Triple(x+1,y,'a'));
    walkerVec.push_back(Triple(x,y+1,'a'));
    walkerVec.push_back(Triple(x+1,y+1,'a'));

    std::unique_ptr<Object> walkerTmp = std::make_unique<Object>(nxtID, 0,walkerVec,State::WALKER,3,12);

    std::unique_ptr<Movement> mvtmp = std::make_unique<MStraight>(2,1+rand()%4);
    walkerTmp->addMv(mvtmp);

    Object* walker = walkerTmp.get();
    grid[x][y].ob.emplace_back(walker);
    walker = walkerTmp.get();
    grid[x+1][y].ob.emplace_back(walker);
    walkerTmp.get();
    grid[x][y+1].ob.emplace_back(walker);
    walkerTmp.get();
    grid[x+1][y+1].ob.emplace_back(walker);
    //oblist.emplace_back(playerTmp);

    //obmap.insert(std::pair<int,std::unique_ptr<Object>>(nxtID, std::move(walkerTmp)));
    obmap.insert(std::pair<int,Object*>(nxtID, walkerTmp.get()));
    oblist.emplace_back(std::move(walkerTmp));
    nxtID++;

    for(auto i:walker->getTriple()){
        refreshCell(i.x,i.y);
    }

}

void Engine::generatePopup(){
    //srand(time(NULL));

    std::vector<Triple> PopupVec;
    int x,y;
    bool ifPlace = true;
    do{
        ifPlace = true;
        x = 1 + rand() % sizeX;
        y = 1 + rand() % sizeY;
        if(!checkEmptyCell(x,y)) ifPlace = false; 
    }while(!ifPlace);
    
    PopupVec.push_back(Triple(x,y,'c'));

    std::unique_ptr<Object> PopupTmp = std::make_unique<Object>(nxtID,0,PopupVec,State::POPUPS,4,60,ObState::INVULNERABLE);

    Object* Popup = PopupTmp.get();
    grid[x][y].ob.emplace_back(Popup);
    //oblist.emplace_back(playerTmp);


    //obmap.insert(std::pair<int,std::unique_ptr<Object>>(nxtID, std::move(exitTmp)));
    obmap.insert(std::pair<int,Object*>(nxtID, PopupTmp.get()));
    oblist.emplace_back(std::move(PopupTmp));
    nxtID++;


    for(auto i:Popup->getTriple()){
        refreshCell(i.x,i.y);
    }

}


void Engine::generateExit(){
    //srand(time(NULL));

    std::vector<Triple> exitVec;
    int x,y;
    bool ifPlace = true;
    do{
        ifPlace = true;
        x = 1 + rand() % (sizeX-1);
        y = 1 + rand() % (sizeY-1);
        if(!checkEmptyCell(x,y)) ifPlace = false; 
    }while(!ifPlace);
    
    exitVec.push_back(Triple(x,y,'e'));

    std::unique_ptr<Object> exitTmp = std::make_unique<Object>(nxtID,0,exitVec,State::EXIT,0);

    Object* exit = exitTmp.get();
    grid[x][y].ob.emplace_back(exit);
    //oblist.emplace_back(playerTmp);


    //obmap.insert(std::pair<int,std::unique_ptr<Object>>(nxtID, std::move(exitTmp)));
    obmap.insert(std::pair<int,Object*>(nxtID, exitTmp.get()));
    oblist.emplace_back(std::move(exitTmp));
    nxtID++;


    for(auto i:exit->getTriple()){
        refreshCell(i.x,i.y);
    }

}


void Engine::generateStalker(){

    std::vector<Triple> stalkerVec;
    int x,y;
    bool ifPlace = true;
    do{
        ifPlace = true;
        x = rand() % sizeX;
        y = rand() % sizeY;
        if(!checkEmptyCell(x,y)) ifPlace = false;
        if(!checkEmptyCell(x+1,y)) ifPlace = false; 
        if(!checkEmptyCell(x,y+1)) ifPlace = false; 
        if(!checkEmptyCell(x+1,y+1)) ifPlace = false;  
    }while(!ifPlace);
    
    stalkerVec.push_back(Triple(x,y,'b'));
    stalkerVec.push_back(Triple(x+1,y,'b'));
    stalkerVec.push_back(Triple(x,y+1,'b'));
    stalkerVec.push_back(Triple(x+1,y+1,'b'));

    std::unique_ptr<Object> stalkerTmp = std::make_unique<Object>(nxtID, 0,stalkerVec,State::STALKER,2,10);

    //std::unique_ptr<Movement> mvtmp = std::make_unique<MStraight>(2,1+rand()%4);
    
    //stalkerTmp->addMv(mvtmp);

    Object* stalker = stalkerTmp.get();
    grid[x][y].ob.emplace_back(stalker);
    stalker = stalkerTmp.get();
    grid[x+1][y].ob.emplace_back(stalker);
    stalkerTmp.get();
    grid[x][y+1].ob.emplace_back(stalker);
    stalkerTmp.get();
    grid[x+1][y+1].ob.emplace_back(stalker);
    //oblist.emplace_back(playerTmp);

    //obmap.insert(std::pair<int,std::unique_ptr<Object>>(nxtID, std::move(walkerTmp)));
    obmap.insert(std::pair<int,Object*>(nxtID, stalkerTmp.get()));
    oblist.emplace_back(std::move(stalkerTmp));
    nxtID++;

    for(auto i:stalker->getTriple()){
        refreshCell(i.x,i.y);
    }
}


void Engine::generateSnake(){
    std::vector<Triple> snakeVec;
    std::unique_ptr<Object> snakeTmp;
    std::unique_ptr<Movement> mvtmp;

    int direc = 1+rand()%4;
    if(direc==1){
        int x,y;
        bool ifPlace = true;
        do{
            ifPlace = true;
            x = rand() % sizeX;
            y = rand() % sizeY;
            if(!checkEmptyCell(x,y)) ifPlace = false;
            if(!checkEmptyCell(x+1,y)) ifPlace = false; 
            if(!checkEmptyCell(x+2,y)) ifPlace = false; 
            if(!checkEmptyCell(x+3,y)) ifPlace = false;  
        }while(!ifPlace);
        snakeVec.push_back(Triple(x,y,'Z'));
        snakeVec.push_back(Triple(x+1,y,'z'));
        snakeVec.push_back(Triple(x+2,y,'z'));
        snakeVec.push_back(Triple(x+3,y,'z'));
        
    }else if(direc == 2){
        int x,y;
        bool ifPlace = true;
        do{
            ifPlace = true;
            x = rand() % sizeX;
            y = rand() % sizeY;
            if(!checkEmptyCell(x,y)) ifPlace = false;
            if(!checkEmptyCell(x-1,y)) ifPlace = false; 
            if(!checkEmptyCell(x-2,y)) ifPlace = false; 
            if(!checkEmptyCell(x-3,y)) ifPlace = false;  
        }while(!ifPlace);
        snakeVec.push_back(Triple(x,y,'Z'));
        snakeVec.push_back(Triple(x-1,y,'z'));
        snakeVec.push_back(Triple(x-2,y,'z'));
        snakeVec.push_back(Triple(x-3,y,'z'));

    }else if(direc == 3){
        int x,y;
        bool ifPlace = true;
        do{
            ifPlace = true;
            x = rand() % sizeX;
            y = rand() % sizeY;
            if(!checkEmptyCell(x,y)) ifPlace = false;
            if(!checkEmptyCell(x,y+1)) ifPlace = false; 
            if(!checkEmptyCell(x,y+2)) ifPlace = false; 
            if(!checkEmptyCell(x,y+3)) ifPlace = false;  
        }while(!ifPlace);
        snakeVec.push_back(Triple(x,y,'Z'));
        snakeVec.push_back(Triple(x,y+1,'z'));
        snakeVec.push_back(Triple(x,y+2,'z'));
        snakeVec.push_back(Triple(x,y+3,'z'));

    }else if(direc == 4){
        int x,y;
        bool ifPlace = true;
        do{
            ifPlace = true;
            x = rand() % sizeX;
            y = rand() % sizeY;
            if(!checkEmptyCell(x,y)) ifPlace = false;
            if(!checkEmptyCell(x,y-1)) ifPlace = false; 
            if(!checkEmptyCell(x,y-2)) ifPlace = false; 
            if(!checkEmptyCell(x,y-3)) ifPlace = false;  
        }while(!ifPlace);
        
        snakeVec.push_back(Triple(x,y,'Z'));
        snakeVec.push_back(Triple(x,y-1,'z'));
        snakeVec.push_back(Triple(x,y-2,'z'));
        snakeVec.push_back(Triple(x,y-3,'z'));
    }
    
    

    snakeTmp = std::make_unique<Object>(nxtID, 0,snakeVec,State::SNAKES,3);
    mvtmp = std::make_unique<MStraight>(5,direc);
    snakeTmp->addMv(mvtmp);

    Object* snake = snakeTmp.get();
    for(auto i : snakeVec){ 
        snake = snakeTmp.get();
        grid[i.x][i.y].ob.emplace_back(snake);
    }


    // grid[x][y].ob.emplace_back(walker);
    // walker = walkerTmp.get();
    // grid[x+1][y].ob.emplace_back(walker);
    // walkerTmp.get();
    // grid[x][y+1].ob.emplace_back(walker);
    // walkerTmp.get();
    // grid[x+1][y+1].ob.emplace_back(walker);
    //oblist.emplace_back(playerTmp);

    //obmap.insert(std::pair<int,std::unique_ptr<Object>>(nxtID, std::move(walkerTmp)));
    obmap.insert(std::pair<int,Object*>(nxtID, snakeTmp.get()));
    oblist.emplace_back(std::move(snakeTmp));
    nxtID++;

    for(auto i: snake->getTriple()){
        refreshCell(i.x,i.y);
    }   
}

void Engine::generateBoss(){
    std::vector<Triple> bossVec;
    int x,y;
    bool ifPlace = true;
    do{
        ifPlace = true;
        x = rand() % sizeX;
        y = rand() % sizeY;

        for(int i=0; i<=4 ;i++){
            if(!checkEmptyCell(x,y+i)) ifPlace = false;
        } 
        
        for(int i=-2;i<=6;i++){
            if(!checkEmptyCell(x+1,y+i)) ifPlace = false;
        }
        for(int i=-3;i<=7;i++){
            if(!checkEmptyCell(x+2,y+i)) ifPlace = false;
        }
        for(int i=-4;i<=8;i++){
            if(!checkEmptyCell(x+3,y+i)) ifPlace = false;
        }
        for(int i=-4;i<=8;i++){
            if(!checkEmptyCell(x+4,y+i)) ifPlace = false;
        }
        for(int i=-3;i<=7;i++){
            if(!checkEmptyCell(x+5,y+i)) ifPlace = false;
        }
        for(int i=-2;i<=6;i++){
            if(!checkEmptyCell(x+6,y+i)) ifPlace = false;
        }  
    }while(!ifPlace);

    bossVec.push_back(Triple(x,y,'-'));
    bossVec.push_back(Triple(x,y+1,'-'));
    bossVec.push_back(Triple(x,y+2,'-'));
    bossVec.push_back(Triple(x,y+3,'-'));
    bossVec.push_back(Triple(x,y+4,'-'));

    // bossVec.push_back(Triple(x+1,y-1,','));
    // bossVec.push_back(Triple(x+1,y,' '));
    // bossVec.push_back(Triple(x+1,y+1,' '));
    // bossVec.push_back(Triple(x+1,y+2,' '));
    // bossVec.push_back(Triple(x+1,y+3,' '));
    // bossVec.push_back(Triple(x+1,y+4,' '));
    // bossVec.push_back(Triple(x+1,y+5,','));

    bossVec.push_back(Triple(x+1,y-2,'.'));
    bossVec.push_back(Triple(x+1,y-1,'\''));
    bossVec.push_back(Triple(x+1,y,' '));
    bossVec.push_back(Triple(x+1,y+1,' '));
    bossVec.push_back(Triple(x+1,y+2,' '));
    bossVec.push_back(Triple(x+1,y+3,' '));
    bossVec.push_back(Triple(x+1,y+4,' '));
    bossVec.push_back(Triple(x+1,y+5,'\''));
    bossVec.push_back(Triple(x+1,y+6,'.'));


    bossVec.push_back(Triple(x+2,y-3,'/'));
    bossVec.push_back(Triple(x+2,y-2,' '));
    bossVec.push_back(Triple(x+2,y-1,' '));
    bossVec.push_back(Triple(x+2,y,'o'));
    bossVec.push_back(Triple(x+2,y+1,' '));
    bossVec.push_back(Triple(x+2,y+2,' '));
    bossVec.push_back(Triple(x+2,y+3,' '));
    bossVec.push_back(Triple(x+2,y+4,'o'));
    bossVec.push_back(Triple(x+2,y+5,' '));
    bossVec.push_back(Triple(x+2,y+6,' '));
    bossVec.push_back(Triple(x+2,y+7,'\\'));


    bossVec.push_back(Triple(x+3,y-4,'|'));
    bossVec.push_back(Triple(x+3,y-3,' '));
    bossVec.push_back(Triple(x+3,y-2,' '));
    bossVec.push_back(Triple(x+3,y-1,' '));
    bossVec.push_back(Triple(x+3,y,' '));
    bossVec.push_back(Triple(x+3,y+1,' '));
    bossVec.push_back(Triple(x+3,y+2,' '));
    bossVec.push_back(Triple(x+3,y+3,' '));
    bossVec.push_back(Triple(x+3,y+4,' '));
    bossVec.push_back(Triple(x+3,y+5,' '));
    bossVec.push_back(Triple(x+3,y+6,' '));
    bossVec.push_back(Triple(x+3,y+7,' '));
    bossVec.push_back(Triple(x+3,y+8,'|'));


    bossVec.push_back(Triple(x+4,y-4,'|'));
    bossVec.push_back(Triple(x+4,y-3,' '));
    bossVec.push_back(Triple(x+4,y-2,' '));
    bossVec.push_back(Triple(x+4,y-1,'\\'));
    bossVec.push_back(Triple(x+4,y,' '));
    bossVec.push_back(Triple(x+4,y+1,' '));
    bossVec.push_back(Triple(x+4,y+2,' '));
    bossVec.push_back(Triple(x+4,y+3,' '));
    bossVec.push_back(Triple(x+4,y+4,' '));
    bossVec.push_back(Triple(x+4,y+5,'/'));
    bossVec.push_back(Triple(x+4,y+6,' '));
    bossVec.push_back(Triple(x+4,y+7,' '));
    bossVec.push_back(Triple(x+4,y+8,'|'));


    bossVec.push_back(Triple(x+5,y-3,'\\'));
    bossVec.push_back(Triple(x+5,y-2,' '));
    bossVec.push_back(Triple(x+5,y-1,' '));
    bossVec.push_back(Triple(x+5,y,','));
    bossVec.push_back(Triple(x+5,y+1,'-'));
    bossVec.push_back(Triple(x+5,y+2,'-'));
    bossVec.push_back(Triple(x+5,y+3,'-'));
    bossVec.push_back(Triple(x+5,y+4,','));
    bossVec.push_back(Triple(x+5,y+5,' '));
    bossVec.push_back(Triple(x+5,y+6,' '));
    bossVec.push_back(Triple(x+5,y+7,'/'));

    // bossVec.push_back(Triple(x+7,y-2,','));
    // bossVec.push_back(Triple(x+7,y-1,' '));
    // bossVec.push_back(Triple(x+7,y,' '));
    // bossVec.push_back(Triple(x+7,y+1,' '));
    // bossVec.push_back(Triple(x+7,y+2,' '));
    // bossVec.push_back(Triple(x+7,y+3,' '));
    // bossVec.push_back(Triple(x+7,y+4,' '));
    // bossVec.push_back(Triple(x+7,y+5,' '));
    // bossVec.push_back(Triple(x+7,y+6,','));

    bossVec.push_back(Triple(x+6,y-2,'\'')); 
    bossVec.push_back(Triple(x+6,y-1,'.'));
    bossVec.push_back(Triple(x+6,y,'-'));
    bossVec.push_back(Triple(x+6,y+1,'-'));
    bossVec.push_back(Triple(x+6,y+2,'-'));
    bossVec.push_back(Triple(x+6,y+3,'-'));
    bossVec.push_back(Triple(x+6,y+4,'-'));
    bossVec.push_back(Triple(x+6,y+5,'.'));
    bossVec.push_back(Triple(x+6,y+6,'\''));
    

    


    std::unique_ptr<Object> bossTmp = std::make_unique<Object>(nxtID, 0,bossVec,State::BOSS,20,100,ObState::PEACE);

    std::unique_ptr<Movement> mvtmp1 = std::make_unique<MStraight>(10,1);
    std::unique_ptr<Movement> mvtmp2 = std::make_unique<MStraight>(10,3);

    bossTmp->addMv(mvtmp1);
    bossTmp->addMv(mvtmp2);

    Object* boss = bossTmp.get();

    for(auto i: bossVec){
        grid[i.x][i.y].ob.emplace_back(boss);
    }
    // grid[x][y].ob.emplace_back(boss);
    // boss = walkerTmp.get();
    // grid[x+1][y].ob.emplace_back(walker);
    // walkerTmp.get();
    // grid[x][y+1].ob.emplace_back(walker);
    // walkerTmp.get();
    // grid[x+1][y+1].ob.emplace_back(walker);


    //oblist.emplace_back(playerTmp);

    //obmap.insert(std::pair<int,std::unique_ptr<Object>>(nxtID, std::move(walkerTmp)));
    obmap.insert(std::pair<int,Object*>(nxtID, bossTmp.get()));
    oblist.emplace_back(std::move(bossTmp));
    nxtID++;

    for(auto i:boss->getTriple()){
        refreshCell(i.x,i.y);
    }
}



bool Engine::checkEmptyCell(int x, int y){
    if(x<=0 || y<=0 || x>=22 || y>=80 || this->grid[x][y].ifWall || this->grid[x][y].ob.size()!=0){
        return false;
    }else{
        return true;
    }
}



