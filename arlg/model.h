#ifndef MODEL_H
#define MODEL_H

#include <memory>
#include <vector>
#include "controller.h"
#include "view.h"

enum Action;

class Model{
  std::vector<std::unique_ptr<View>> views;
  std::unique_ptr<Controller> control;
 protected:
  void addView(std::unique_ptr<View> v);
  void destroyViews();
  void addController(std::unique_ptr<Controller> v);
  Action getAction();
 public:
  virtual ~Model() = default;
  void updateViews(int, int, State);
  void updateViews(int i, const std::string& s);
  void updateViews(int,int,char,Color c = Color::WHITE);
  void displayViews();
};

#endif


