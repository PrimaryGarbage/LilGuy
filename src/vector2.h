#ifndef __VECTOR2_H__
#define __VECTOR2_H__

#include "short_types.h"

typedef struct Vector2 {
    float x;
    float y;
} Vector2;

typedef struct Vector2_i {
    i32 x;
    i32 y;
} Vector2_i;

typedef struct Vector2_u {
    u32 x;
    u32 y;
} Vector2_u;

Vector2 Vector2_zero();

Vector2 Vector2_AddScalar(Vector2 vec, float value);
Vector2_i Vector2i_AddScalar(Vector2_i vec, i32 value);
Vector2_u Vector2u_AddScalar(Vector2_u vec, u32 value);

Vector2 Vector2_Div(Vector2 vec, float value);

#endif // __VECTOR2_H__