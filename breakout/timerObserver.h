#ifndef TIMEROBSERVER_H
#define TIMEROBSERVER_H

class TimerObserver{
public:
    virtual void timeNotify() = 0;
    virtual ~TimerObserver() = default;
};

#endif

