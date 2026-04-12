#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "vector2.h"

typedef struct Transform {
    Vector2 position;
    float rotation;
    Vector2 scale;
    Vector2 origin;
    bool topLevel;
} Transform;

static inline Transform Transform_Zero()
{
    return (Transform) {
        .position = Vector2_Zero(),
        .origin = Vector2_Zero(),
        .rotation = 0.0f,
        .scale = Vector2_Uniform(1.0f)
    };
}

static inline Transform Transform_Combine(Transform* left, Transform* right)
{
    return (Transform) {
        .position = Vector2_Add(left->position, right->position),
        .origin = right->origin,
        .rotation = left->rotation + right->rotation,
        .scale = Vector2_Mult(left->scale, right->scale)
    };
}

#endif // __TRANSFORM_H__