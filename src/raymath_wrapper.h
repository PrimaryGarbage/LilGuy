#ifndef __RAYMATH_WRAPPER_H__
#define __RAYMATH_WRAPPER_H__

#include "physics/matrix.h"
#include "vector2.h"

Matrix Raymath_MatrixIdentity();
Matrix Raymath_MatrixTranslate(Vector2 translation);
Matrix Raymath_MatrixRotate(float angle);
Matrix Raymath_MatrixScale(Vector2 scale);
Matrix Raymath_MatrixMult(const Matrix* left, const Matrix* right);

#endif // __RAYMATH_WRAPPER_H__