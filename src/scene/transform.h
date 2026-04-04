#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "vector2.h"

typedef struct Transform {
    Vector2 position;
    float rotation;
    Vector2 scale;
} Transform;

#endif // __TRANSFORM_H__