#ifndef __VECTOR2_H__
#define __VECTOR2_H__

#include "short_types.h"

typedef struct Vector2 {
    float x;
    float y;
} Vector2;

typedef struct Vector2i {
    i32 x;
    i32 y;
} Vector2i;

typedef struct Vector2u {
    u32 x;
    u32 y;
} Vector2u;

Vector2 Vector2_Zero();

Vector2 Vector2_AddScalar(Vector2 vec, float value);
Vector2i Vector2i_AddScalar(Vector2i vec, i32 value);
Vector2u Vector2u_AddScalar(Vector2u vec, u32 value);

Vector2 Vector2_Div(Vector2 vec, float value);

#endif // __VECTOR2_H__