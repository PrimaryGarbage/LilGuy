#include "math_helpers.h"
#include <math.h>

#define CLAMP(min, max, val) (MAX(MIN(val, max), min))

i32 Clampi(i32 min, i32 max, i32 val)
{
    return CLAMP(min, max, val);
}

u32 Clampu(u32 min, u32 max, u32 val)
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

float LerpAnglef(float a, float b, float w)
{
    if (fabs(b-a) > 180.0f)
        b += 360.0f;

    float result = a + (b - a) * w;

    if (result > 180.0f)
        result -= 360.0f;

    return result;
}

float Signf(float val)
{
    return val < 0.0f? -1.0f : 1.0f;
}
