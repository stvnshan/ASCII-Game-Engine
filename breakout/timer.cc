#include "timer.h"
#include "timerSubject.h"
#include <chrono>
#include <ratio>
#include <iostream>


void Timer::startTime(){
    timer = std::chrono::system_clock::now();
    while(true){
        std::chrono::duration<int, std::milli> time_dur = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - timer);
        
        if(time_dur.count() >= 50){
            //std::cout<< time_dur.count()<<std::endl;
            notifyTimerObservers();
            timer = std::chrono::system_clock::now();
            //timer = timer + 0.05;   
        }
    }
    
}

