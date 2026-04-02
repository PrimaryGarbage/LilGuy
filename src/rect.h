#ifndef __RECT_H__
#define __RECT_H__

#include "vector2.h"

typedef struct Rect {
    Vector2 a;
    Vector2 b;
} Rect;

typedef struct Rect_i {
    Vector2_i a;
    Vector2_i b;
} Rect_i;

typedef struct Rect_u {
    Vector2_u a;
    Vector2_u b;
} Rect_u;

#endif // __RECT_H__