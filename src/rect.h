#ifndef __RECT_H__
#define __RECT_H__

#include "short_types.h"

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

#endif // __RECT_H__