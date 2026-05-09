#ifndef __VECTOR3_H__
#define __VECTOR3_H__

#include "vector2.h"

typedef struct {
    float x, y, z;
} Vector3;

Vector3 Vector3_FromVector2(Vector2 vec);

#endif // __VECTOR3_H__