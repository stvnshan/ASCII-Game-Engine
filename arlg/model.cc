#include "model.h"
#include "curseControl.h"
#include "curseView.h"


void Model::updateViews(int row, int col, State s){
  for( auto& view: views ){
    view->update(row, col, s);
  }
}

void Model::updateViews(int row, int col, char ch, Color c){
  for( auto& view: views ){
    view->update(row, col, ch, c);
  }
}

void Model::updateViews(int i, const std::string& s){
  for( auto& view: views ){
    view->update(i,s);
  }
}

void Model::displayViews(){
  for( auto& view: views ){
    view->displayView();
  }
}

void Model::addView(std::unique_ptr<View> v){
  views.emplace_back(std::move(v));
}
void Model::addController(std::unique_ptr<Controller> c){
  control = std::move(c);
}

Action Model::getAction(){
  return control->getAction();
}

void Model::destroyViews(){
  views.clear();
}


