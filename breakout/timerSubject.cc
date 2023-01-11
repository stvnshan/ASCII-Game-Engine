#include <time.h>
#include "timerSubject.h"
#include <vector>
#include <algorithm>

void TimerSubject::notifyTimerObservers(){
    for (auto &ob : tobs) ob->timeNotify();
}

void TimerSubject::attach(TimerObserver *to){
    tobs.emplace_back(to);
}

void TimerSubject::detach(TimerObserver *to){
    tobs.erase(std::remove(tobs.begin(), tobs.end(), to), tobs.end());
}


