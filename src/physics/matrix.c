#include "matrix.h"
#include "raymath_wrapper.h"

Matrix Matrix_Translate(Vector2 translation)
{
    return Raymath_MatrixTranslate(translation);
}

Matrix Matrix_Rotate(float angle)
{
    return Raymath_MatrixRotate(angle);
}

Matrix Matrix_Scale(Vector2 scale)
{
    return Raymath_MatrixScale(scale);
}

Matrix Matrix_Mult(const Matrix* left, const Matrix* right)
{
    return Raymath_MatrixMult(left, right);
}

Matrix Matrix_Identity()
{
    return Raymath_MatrixIdentity();
}
