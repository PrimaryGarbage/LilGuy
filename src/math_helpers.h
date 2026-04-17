#ifndef __MATH_HELPERS_H__
#define __MATH_HELPERS_H__

#include "short_types.h"

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) > (b)) ? (b) : (a))
#define SQUARED(v) ((v) * (v))

i32 Clampi(i32 min, i32 max, i32 val);
u32 Clampu(u32 min, u32 max, u32 val);
float Clampf(float min, float max, float val);
float Lerpf(float a, float b, float w);
// angles from -180 to 180
float LerpAnglef(float a, float b, float w);
float Signf(float val);

static inline float DegToRad(float deg)
{
    constexpr float twoPi = 2.0f * 3.14159265f;

    return twoPi * deg / 360.0f;
}

static inline float RadToDeg(float rad)
{
    constexpr float twoPi = 2.0f * 3.14159265f;

    return 360.0f * rad / twoPi;
}

#endif // __MATH_HELPERS_H__