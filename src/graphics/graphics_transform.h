#ifndef __GRAPHICS_TRANSFORM_H__
#define __GRAPHICS_TRANSFORM_H__

#include "vector2.h"

typedef struct GraphicsTransform {
    Vector2 position;
    float rotation;
    Vector2 scale;
    Vector2 origin;
} GraphicsTransform;

#endif // __GRAPHICS_TRANSFORM_H__