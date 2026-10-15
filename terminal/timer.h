#ifndef TIMER_H
#define TIMER_H

void timer(unsigned int deciseconds);
int msTimeoutC(unsigned int milliseconds);
static inline int timeoutC(unsigned int deciseconds)
{
    return msTimeoutC(deciseconds*100);
}

#endif