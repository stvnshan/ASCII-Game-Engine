#ifndef TIMERSUBJECT_H
#define TIMERSUBJECT_H
#include <time.h>
#include <vector>
#include <algorithm>
#include "timerObserver.h"

class TimerSubject{
    std::vector<TimerObserver*> tobs;

public:
    void notifyTimerObservers();
    void attach(TimerObserver *to);
    void detach(TimerObserver *to);
};



#endif


