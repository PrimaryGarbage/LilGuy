#include "vector2.h"
#include <math.h>

Vector2 Vector2_Zero()
{
    return (Vector2) { 0.0f , 0.0f };
}

Vector2 Vector2_One()
{
    return (Vector2) { 1.0f , 1.0f };
}

Vector2 Vector2_Uniform(float value)
{
    return (Vector2) { value , value };
}

Vector2 Vector2_New(float x, float y)
{
    return (Vector2){ x, y };
}

Vector2 Vector2_AddScalar(Vector2 vec, float value)
{
    return (Vector2){ vec.x + value, vec.y + value };
}

Vector2i Vector2i_AddScalar(Vector2i vec, i32 value)
{
    return (Vector2i){ vec.x + value, vec.y + value };
}

Vector2u Vector2u_AddScalar(Vector2u vec, u32 value)
{
    return (Vector2u){ vec.x + value, vec.y + value };
}

Vector2 Vector2_MultScalar(Vector2 vec, float value)
{
    return (Vector2){ vec.x * value, vec.y * value };
}

Vector2 Vector2_DivScalar(Vector2 vec, float value)
{
    return (Vector2){ vec.x / value, vec.y / value };
}

Vector2 Vector2_Add(Vector2 left, Vector2 right)
{
    return (Vector2){ left.x + right.x, left.y + right.y };
}

Vector2 Vector2_Sub(Vector2 left, Vector2 right)
{
    return (Vector2){ left.x - right.x, left.y - right.y };
}

Vector2 Vector2_Mult(Vector2 left, Vector2 right)
{
    return (Vector2){ left.x * right.x, left.y * right.y };
}

Vector2 Vector2_Div(Vector2 left, Vector2 right)
{
    return (Vector2){ left.x / right.x, left.y / right.y };
}

float Vector2_Length(Vector2 vec)
{
    return sqrtf(vec.x * vec.x + vec.y * vec.y);
}

float Vector2_LengthSquared(Vector2 vec)
{
    
    return vec.x * vec.x + vec.y * vec.y;
}

Vector2 Vector2_Lerp(Vector2 a, Vector2 b, float w)
{
    return (Vector2) {
        .x = a.x + (b.x - a.x) * w,
        .y = a.y + (b.y - a.y) * w
    };
}

bool Vector2_Equals(Vector2 left, Vector2 right)
{
    return left.x == right.x && left.y == right.y;
}

bool Vector2_EqualsWithin(Vector2 left, Vector2 right, float delta)
{
    return fabsf(left.x - right.x) < delta && fabsf(left.y - right.y) < delta;
}