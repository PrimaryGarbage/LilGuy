#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "short_types.h"
#include "vector2.h"

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
    size_t dataSize;
} Image;

void Image_Free(Image* image);
u32 Image_SampleNormalized(const Image* image, float x, float y);
u32 Image_SampleNormalizedV(const Image* image, Vector2 point);
Image Image_CreateResized(const Image* image, u32 newWidth, u32 newHeight);
void Image_SwapRAndBChannels(Image* image);

#endif // __IMAGE_H__