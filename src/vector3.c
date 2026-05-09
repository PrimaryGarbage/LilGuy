#include "vector3.h"

Vector3 Vector3_FromVector2(Vector2 vec)
{
    return (Vector3){ .x = vec.x, .y = vec.y, .z = 0.0f };
}