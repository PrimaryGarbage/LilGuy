#ifndef __RECT_H__
#define __RECT_H__

#include "short_types.h"
#include "vector2.h"

typedef struct Rect {
    float x;
    float y;
    float width;
    float height;
} Rect;

typedef struct Recti {
    i32 x;
    i32 y;
    i32 width;
    i32 height;
} Rect_i;

typedef struct Rectu {
    u32 x;
    u32 y;
    u32 width;
    u32 height;
} Rect_u;

static inline Rect Rect_FromVectors(Vector2 pos, Vector2 size)
{
    return (Rect){
        .x = pos.x,
        .y = pos.y,
        .width = size.x,
        .height = size.y
    };
}

static inline Vector2 Rect_GetPosition(const Rect* rect)
{
    return (Vector2){ .x = rect->x, .y = rect->y };
}

static inline Vector2 Rect_GetSize(const Rect* rect)
{
    return (Vector2){ .x = rect->width, .y = rect->height };
}

#endif // __RECT_H__