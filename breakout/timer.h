#ifndef TIMER_H
#define TIMER_H
#include "timerSubject.h"
#include <chrono>
#include <ctime>

class Timer: public TimerSubject{
    
    std::chrono::time_point<std::chrono::system_clock> timer;
public:
    void startTime();
    //int getTime();
};
#endif
