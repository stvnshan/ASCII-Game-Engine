#ifndef VIEW_H
#define VIEW_H

#include <string>
#include "state.h"
#include "color.h"

class View{
 public:
  virtual ~View() = default;
  virtual void update(int i, const std::string& msg) = 0;
  virtual void update(int row, int col, State state,Color c = Color::WHITE) = 0;
  virtual void update(int row, int col, char ch, Color c = Color::WHITE) = 0;
  virtual void displayView() = 0;
};

#endif


