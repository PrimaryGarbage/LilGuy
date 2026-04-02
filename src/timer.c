#include "timer.h"
#include <time.h>

static double GetCurrentTimeInSeconds()
{
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return ((double)ts.tv_sec) + ((float)ts.tv_nsec) * 0.000000001;
}

Timer Timer_Create()
{
    return (Timer) {
        .started = GetCurrentTimeInSeconds()
    };
}

double Timer_Reset(Timer* timer)
{
    double elapsed = Timer_Elapsed(timer);
    timer->started = GetCurrentTimeInSeconds();
    return elapsed;
}

double Timer_Elapsed(const Timer* timer)
{
    return GetCurrentTimeInSeconds() - timer->started;
}
