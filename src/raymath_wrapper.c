#define Matrix RlMatrix
#define Vector2 RlVector2
#include "raymath.h"
#undef Matrix
#undef Vector2

#include "raymath_wrapper.h"
#include <string.h>

#define TYPE_PUN(typeFrom, typeTo, valueFrom, valueTo) \
do { \
    static_assert(sizeof(typeFrom) == sizeof(typeTo), "Struct sizes mismatch!"); \
    static_assert(_Alignof(typeFrom) == _Alignof(typeTo), "Struct alignments mismatch!"); \
    memcpy(&valueTo, &valueFrom, sizeof(typeTo)); \
} while(0)

static inline RlMatrix MatrixToRlMatrix(Matrix in)
{
    RlMatrix out;
    TYPE_PUN(Matrix, RlMatrix, in, out);
    return out;
}

static inline Matrix RlMatrixToMatrix(RlMatrix in)
{
    Matrix out;
    TYPE_PUN(RlMatrix, Matrix, in, out);
    return out;
}

Matrix Raymath_MatrixTranslate(Vector2 translation)
{
    return RlMatrixToMatrix(MatrixTranslate(translation.x, translation.y, 0.0f));
}

Matrix Raymath_MatrixRotate(float angle)
{
    return RlMatrixToMatrix(MatrixRotateZ(angle));
}

Matrix Raymath_MatrixScale(Vector2 scale)
{
    return RlMatrixToMatrix(MatrixScale(scale.x, scale.y, 0.0f));
}

Matrix Raymath_MatrixMult(const Matrix* left, const Matrix* right)
{
    return RlMatrixToMatrix(MatrixMultiply(MatrixToRlMatrix(*left), MatrixToRlMatrix(*right)));
}

Matrix Raymath_MatrixIdentity()
{
    return RlMatrixToMatrix(MatrixIdentity());
}
