#include "math_helpers.h"

#define CLAMP(min, max, val) (MAX(MIN(val, max), min))

i32 Clampi(i32 min, i32 max, i32 val)
{
    return CLAMP(min, max, val);
}

u32 Clampu(i32 min, u32 max, u32 val)
{
    return CLAMP(min, max, val);
}

float Clampf(float min, float max, float val)
{
    return CLAMP(min, max, val);
}

float Lerpf(float a, float b, float w)
{
    return a + (b - a) * w;
}

float Signf(float val)
{
    return val < 0.0f? -1.0f : 1.0f;
}
