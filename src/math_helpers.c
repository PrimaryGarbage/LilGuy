#include "math_helpers.h"

#define CLAMP(val, min, max) (MAX(MIN(val, max), min))

i32 Clampi(i32 val, i32 min, i32 max)
{
    return CLAMP(val, min, max);
}

u32 Clampu(i32 val, u32 min, u32 max)
{
    return CLAMP(val, min, max);
}

float Clampf(float val, float min, float max)
{
    return CLAMP(val, min, max);
}