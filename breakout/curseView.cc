#include "curseView.h"
#include <ncurses.h>
//#include "maze.h"

Curses::Curses(){
  setlocale(LC_ALL, "");
  initscr();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);
  start_color();
  init_pair(1, COLOR_WHITE,COLOR_BLACK);
  init_pair(2, COLOR_RED,COLOR_BLACK);
  init_pair(3, COLOR_BLUE,COLOR_BLACK);
  init_pair(4, COLOR_GREEN,COLOR_BLACK);
  init_pair(5, COLOR_CYAN,COLOR_BLACK);
  displayMap[EMPTY] = ' ';
  displayMap[WALL] = '#';
  //displayMap[GOAL] = '*';
  displayMap[PLAYER] = 'p';
  
  // exterior walls
  for (int i = 1; i < cols - 1; ++i){
    mvwaddch(stdscr, 0, i, '-' );
    mvwaddch(stdscr, rows-1, i, '-');
  }
  for (int i = 1; i < rows - 1; ++i){
    mvwaddch(stdscr, i, 0, '|');
    mvwaddch(stdscr, i, cols-1, '|');
  }
  mvwaddch(stdscr, 0, 0, '+');
  mvwaddch(stdscr, 0, cols-1, '+');
  mvwaddch(stdscr, rows-1, 0, '+');
  mvwaddch(stdscr, rows-1, cols-1, '+');
  //refresh();
}


void Curses::displayView(){
  refresh();
}

void Curses::update(int i, const std::string& msg){
  wmove(stdscr, rows+i,0);
  waddstr(stdscr,"                                              ");
  wmove(stdscr, rows+i,0);
  waddstr(stdscr,msg.c_str());
}


void Curses::update(int row, int col, State state, Color c){
  if(displayMap[state] == 'P'){
    c = Color::RED;
  }
  wmove(stdscr,row,col);
  if(c == Color::WHITE){
    waddch(stdscr,displayMap[state]|COLOR_PAIR(1));
  }else if(c == Color::RED){
    waddch(stdscr,displayMap[state]|COLOR_PAIR(2));
  }else if(c == Color::BLUE){
    waddch(stdscr,displayMap[state]|COLOR_PAIR(3));
  }else if(c == Color::GREEN){
    waddch(stdscr,displayMap[state]|COLOR_PAIR(4));
  }
  
  //mvwaddch(stdscr,row,col,displayMap[state]);
}

void Curses::update(int row, int col, char ch, Color c){
  if(c==Color::WHITE){
    if(ch == 'p'){
      c = Color::RED;
    }else if(ch=='O'|| ch=='F' || ch=='G' || ch=='!'){
      c = Color::CYAN;
    }else if(ch == 'X'){
      c = Color::GREEN;
    }
  }
  //start_color();
  wmove(stdscr,row,col);
  if(c == Color::WHITE){
    waddch(stdscr,ch|COLOR_PAIR(1));
  }else if(c == Color::RED){
    waddch(stdscr,ch|COLOR_PAIR(2));
  }else if(c == Color::BLUE){
    waddch(stdscr,ch|COLOR_PAIR(3));
  }else if(c == Color::GREEN){
    waddch(stdscr,ch|COLOR_PAIR(4));
  }else if(c == Color::CYAN){
    waddch(stdscr,ch|COLOR_PAIR(5));
  }
  // mvwaddch(stdscr,row,col,ch);
}

Curses::~Curses(){ 
  endwin();
}


