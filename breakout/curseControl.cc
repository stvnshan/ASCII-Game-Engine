#include "curseControl.h"
#include <string>

CurseKeyboard::CurseKeyboard() {
    setlocale(LC_ALL, "");
    mapping['w'] = Action::W;
    mapping['d'] = Action::D;
    mapping['s'] = Action::S;
    mapping['a'] = Action::A;
    mapping[KEY_UP] = Action::UP;
    mapping[KEY_RIGHT] = Action::RIGHT;
    mapping[KEY_DOWN] = Action::DOWN;
    mapping[KEY_LEFT] = Action::LEFT;
}

Action CurseKeyboard::action(){
    // int n;
    // while ( (n = getch()) == ERR ) continue;

    // if ( mapping.find(n) != mapping.end() ){
    //     return mapping[n];
    // }
    // return Action::D;
    
    int ch;
    //cbreak();
    nodelay(stdscr, TRUE);
    
    for (;;) {
        if ((ch = getch()) == ERR) {
              return Action::INVALID;
        }else {
            if ( mapping.find(ch) != mapping.end() ){
                return mapping[ch];
            }else{
                return Action::INVALIDD;
            }
        }    
    }
  
}

