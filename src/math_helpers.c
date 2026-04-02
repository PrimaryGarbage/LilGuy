#include "math_helpers.h"

#define CLAMP(val, min, max) (MAX(MIN(val, max), min))

i32 clamp_i(i32 val, i32 min, i32 max)
{
    return CLAMP(val, min, max);
}

u32 clamp_u(i32 val, u32 min, u32 max)
{
    return CLAMP(val, min, max);
}

float clamp_f(float val, float min, float max)
{
    return CLAMP(val, min, max);
}