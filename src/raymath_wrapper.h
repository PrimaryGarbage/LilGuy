#ifndef __RAYMATH_WRAPPER_H__
#define __RAYMATH_WRAPPER_H__

#include "physics/matrix.h"
#include "vector2.h"

typedef struct float16 {
    float v[16];
} float16;

Matrix Raymath_MatrixIdentity();
Matrix Raymath_MatrixTranslate(Vector2 translation);
Matrix Raymath_MatrixRotate(float angle);
Matrix Raymath_MatrixScale(Vector2 scale);
Matrix Raymath_MatrixMult(const Matrix* left, const Matrix* right);
float16 Raymath_MatrixToFloatV(const Matrix* matrix);

#define Raymath_MatrixToFloat(mat) (Raymath_MatrixToFloatV(mat).v)

#endif // __RAYMATH_WRAPPER_H__