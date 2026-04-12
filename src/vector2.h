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

Vector2 Vector2_New(float x, float y);
Vector2 Vector2_Zero();
Vector2 Vector2_One();
Vector2 Vector2_Uniform(float value);

Vector2 Vector2_AddScalar(Vector2 vec, float value);
Vector2i Vector2i_AddScalar(Vector2i vec, i32 value);
Vector2u Vector2u_AddScalar(Vector2u vec, u32 value);
Vector2 Vector2_MultScalar(Vector2 vec, float value);
Vector2 Vector2_DivScalar(Vector2 vec, float value);
Vector2 Vector2_Add(Vector2 left, Vector2 right);
Vector2 Vector2_Sub(Vector2 left, Vector2 right);
Vector2 Vector2_Mult(Vector2 left, Vector2 right);
Vector2 Vector2_Div(Vector2 left, Vector2 right);
Vector2 Vector2_Rotate(Vector2 vec, float angle);
Vector2 Vector2_Normalize(Vector2 vec);
float Vector2_Length(Vector2 vec);
float Vector2_LengthSquared(Vector2 vec);
Vector2 Vector2_Lerp(Vector2 a, Vector2 b, float w);
bool Vector2_Equals(Vector2 left, Vector2 right);
bool Vector2_EqualsWithin(Vector2 left, Vector2 right, float delta);

#endif // __VECTOR2_H__