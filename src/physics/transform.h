#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

#include "math_helpers.h"
#include "matrix.h"
#include "vector2.h"
#include <assert.h>
#include <math.h>

typedef struct Transform {
    Vector2 position;
    float rotation;
    Vector2 scale;
    Vector2 origin;
    bool topLevel;
    Matrix matrix;
} Transform;

static inline Transform Transform_Zero()
{
    return (Transform) {
        .position = Vector2_Zero(),
        .origin = Vector2_Zero(),
        .rotation = 0.0f,
        .scale = Vector2_Uniform(1.0f),
        .topLevel = false,
        .matrix = Matrix_Identity()
    };
}

//static inline Transform Transform_Combine(Transform* left, Transform* right)
//{
//    return (Transform) {
//        .position = Vector2_Add(left->position, right->position),
//        .origin = right->origin,
//        .rotation = left->rotation + right->rotation,
//        .scale = Vector2_Mult(left->scale, right->scale),
//    };
//}

static inline Matrix Transform_CreateMatrix(const Transform* transform)
{
    Matrix scale = Matrix_Scale(transform->scale);
    Matrix rotation = Matrix_Rotate(DegToRad(transform->rotation));
    Matrix translation = Matrix_Translate(transform->position);


    Matrix result = Matrix_Identity();
    result = Matrix_Mult(&result, &scale);
    result = Matrix_Mult(&result, &rotation);
    result = Matrix_Mult(&result, &translation);

    return result;
}

static inline void Transform_UpdateMatrix(Transform* transform)
{
    transform->matrix = Transform_CreateMatrix(transform);
}

static inline Transform Transform_FromMatrix(const Matrix* matrix)
{
    float scaleX = sqrtf(matrix->m0 * matrix->m0 + matrix->m1 * matrix->m1);
    float scaleY = sqrtf(matrix->m4 * matrix->m4 + matrix->m5 * matrix->m5);

    return (Transform){
        .position = (Vector2){ .x = matrix->m12, .y = matrix->m13 },
        .rotation = RadToDeg(atan2f(matrix->m1, matrix->m0)),
        .scale = (Vector2){ .x = scaleX, .y = scaleY },
        .matrix = *matrix,
        .topLevel = false,
        .origin = Vector2_Zero()
    };
}

#endif // __TRANSFORM_H__