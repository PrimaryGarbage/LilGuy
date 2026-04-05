#include "vector2.h"

Vector2 Vector2_Zero()
{
    return (Vector2) { 0.0f , 0.0f };
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

Vector2 Vector2_Div(Vector2 vec, float value)
{
    return (Vector2){ vec.x / value, vec.y / value };
}
