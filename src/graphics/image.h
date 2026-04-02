#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "short_types.h"
#include "vector2.h"
#include "color.h"

typedef enum PixelFormat {
    PIXEL_FORMAT_RGBA,
    PIXEL_FORMAT_BGRA,
} PixelFormat;

// Image with BGRA data
typedef struct Image {
    u32 width;
    u32 height;
    PixelFormat format;
    u32* data;
} Image;

void Image_Free(Image* image);
Color Image_SampleNormalized(const Image* image, float x, float y);
Color Image_SampleNormalizedV(const Image* image, Vector2 point);

#endif // __IMAGE_H__