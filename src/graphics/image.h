#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "short_types.h"

// Image with BGRA data
typedef struct Image {
    u32 width;
    u32 height;
    u32* data;
} Image;

void Image_Free(Image* image);

#endif // __IMAGE_H__