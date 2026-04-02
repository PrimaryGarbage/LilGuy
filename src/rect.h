#ifndef __RECT_H__
#define __RECT_H__

#include "vector2.h"

typedef struct Rect {
    Vector2 a;
    Vector2 b;
} Rect;

typedef struct Rect_i {
    Vector2i a;
    Vector2i b;
} Rect_i;

typedef struct Rect_u {
    Vector2u a;
    Vector2u b;
} Rect_u;

static inline Rect Rect_Create(float x, float y, float width, float height)
{
    return (Rect) {
        .a = (Vector2){ x, y },
        .b = (Vector2){ x + width, y + height }
    };
}

static inline Rect Rect_CreateSquare(float x, float y, float size)
{
    return Rect_Create(x, y, size, size);
}

#endif // __RECT_H__