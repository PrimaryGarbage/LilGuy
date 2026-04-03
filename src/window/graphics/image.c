#include <stdlib.h>
#include <math.h>
#include "image.h"
#include "logging.h"

void Image_Free(Image* image)
{
    free(image->data);
}

Color Image_SampleNormalized(const Image* image, float x, float y)
{
    if (x > 1.0f || y > 1.0f)
        LogWarning("Using Image_SampleNormalized function with values outside of 0..1 range");

    u32 bufferX = (u32)roundf(x * (image->width - 1));
    u32 bufferY = (u32)roundf(y * (image->height - 1));

    return Color_Unpack(image->data[bufferX + image->width * bufferY]);
}

Color Image_SampleNormalizedV(const Image* image, Vector2 point)
{
    return Image_SampleNormalized(image, point.x, point.y);
}