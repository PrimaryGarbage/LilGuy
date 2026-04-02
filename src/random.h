#ifndef __RANDOM_H__
#define __RANDOM_H__

#include <stdlib.h>
#include <time.h>
#include "short_types.h"

static inline void RandomInit()
{
    srand(time(NULL));
}

// from 0.0 to 1.0
static inline float RandomFloat()
{
    return (float)rand() / (float)RAND_MAX;
}

static inline i32 RandomRange(i32 from, i32 to)
{
    return RandomFloat() * (to - from);
}

#endif // __RANDOM_H__