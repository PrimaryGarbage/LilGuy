#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "vector2.h"
typedef struct Matrix {
    float m0, m4, m8, m12;  // Matrix first row (4 components)
    float m1, m5, m9, m13;  // Matrix second row (4 components)
    float m2, m6, m10, m14; // Matrix third row (4 components)
    float m3, m7, m11, m15; // Matrix fourth row (4 components)
} Matrix;

Matrix Matrix_Translate(Vector2 translation);
Matrix Matrix_Rotate(float angle);
Matrix Matrix_Scale(Vector2 scale);
Matrix Matrix_Mult(const Matrix* left, const Matrix* right);
Matrix Matrix_Identity();

#endif // __MATRIX_H__