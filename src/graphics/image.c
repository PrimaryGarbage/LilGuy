#include <stdlib.h>
#include <math.h>
#include "image.h"
#include "color.h"
#include "result.h"
#include "raylib_wrapper.h"

void Image_Free(Image* image)
{
    free(image->data);
}

u32 Image_SampleNormalized(const Image* image, float x, float y)
{
    u32 bufferX = (u32)roundf(x * (image->width - 1));
    u32 bufferY = (u32)roundf(y * (image->height - 1));

    return image->data[bufferX + image->width * bufferY];
}

u32 Image_SampleNormalizedV(const Image* image, Vector2 point)
{
    return Image_SampleNormalized(image, point.x, point.y);
}

Image Image_CreateResized(const Image* image, u32 newWidth, u32 newHeight)
{
    Image newImage = {
        .data = malloc(newWidth * newHeight * 4),
        .width = newWidth,
        .height = newHeight,
        .format = image->format
    };

    if (!newImage.data) MEMORY_PANIC();

    for (u32 i = 0u; i < image->width; ++i)
        for (u32 j = 0u; j < image->height; ++j)
            newImage.data[i + newWidth * j] = Image_SampleNormalized(image, (float)i / (float)newWidth, (float)j / (float)newHeight);
    
    return newImage;
}

void Image_SwapRAndBChannels(Image* image)
{
    for(size_t i = 0u; i < image->width * image->height; ++i)
        image->data[i] = Color_SwapRAndB(image->data[i]);
}

Image Image_Load(const char* path)
{
    return Raylib_LoadImage(path);
}

void Image_FlipVertical(Image* image)
{
    Raylib_ImageFlipVertical(image);
}

void Image_FlipHorizontal(Image* image)
{
    Raylib_ImageFlipHorizontal(image);
}