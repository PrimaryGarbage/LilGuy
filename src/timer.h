#ifndef __TIMER_H__
#define __TIMER_H__

typedef struct Timer {
    double started;
} Timer;

Timer Timer_Create();
double Timer_Reset(Timer* timer);
double Timer_Elapsed(const Timer* timer);

#endif // __TIMER_H__