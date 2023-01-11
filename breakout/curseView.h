#ifndef CURSESVIEW_H
#define CURSESVIEW_H

#include "view.h"
#include <vector>
#include <iostream>
#include <map>
#include "state.h"


class Maze;

class Curses: public View{
  int cols = 80;
  int rows = 22;
  std::map<State, char> displayMap;
  bool firstDisplay = true;
  
 public:
  Curses();
  ~Curses();
  void update(int i, const std::string& msg) override;
  void update(int row, int col, State state, Color c = Color::WHITE) override;
  void update(int row, int col, char ch, Color c = Color::WHITE) override;
  void displayView() override;
};
#endif

