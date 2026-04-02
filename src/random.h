#ifndef __RANDOM_H__
#define __RANDOM_H__

#include <stdlib.h>
#include <time.h>
#include "short_types.h"

static inline void randomInit()
{
    srand(time(NULL));
}

// from 0.0 to 1.0
static inline float randomFloat()
{
    return (float)rand() / (float)RAND_MAX;
}

static inline i32 randomRange(i32 from, i32 to)
{
    return randomFloat() * (to - from);
}

#endif // __RANDOM_H__