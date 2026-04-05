#ifndef __TEXTURE2D_H__
#define __TEXTURE2D_H__

#include "graphics/image.h"
#include "short_types.h"

typedef struct Texture2D {
    u32 id;
    u32 width;
    u32 height;
    PixelFormat format;
} Texture2D;

#endif // __TEXTURE2D_H__