#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "short_types.h"
#include "vector2.h"

typedef enum PixelFormat {
    PIXEL_FORMAT_UNCOMPRESSED_GRAYSCALE = 1, // 8 bit per pixel (no alpha)
    PIXEL_FORMAT_UNCOMPRESSED_GRAY_ALPHA,    // 8*2 bpp (2 channels)
    PIXEL_FORMAT_UNCOMPRESSED_R5G6B5,        // 16 bpp
    PIXEL_FORMAT_UNCOMPRESSED_R8G8B8,        // 24 bpp
    PIXEL_FORMAT_UNCOMPRESSED_R5G5B5A1,      // 16 bpp (1 bit alpha)
    PIXEL_FORMAT_UNCOMPRESSED_R4G4B4A4,      // 16 bpp (4 bit alpha)
    PIXEL_FORMAT_UNCOMPRESSED_R8G8B8A8,      // 32 bpp
    PIXEL_FORMAT_UNCOMPRESSED_R32,           // 32 bpp (1 channel - float)
    PIXEL_FORMAT_UNCOMPRESSED_R32G32B32,     // 32*3 bpp (3 channels - float)
    PIXEL_FORMAT_UNCOMPRESSED_R32G32B32A32,  // 32*4 bpp (4 channels - float)
    PIXEL_FORMAT_UNCOMPRESSED_R16,           // 16 bpp (1 channel - half float)
    PIXEL_FORMAT_UNCOMPRESSED_R16G16B16,     // 16*3 bpp (3 channels - half float)
    PIXEL_FORMAT_UNCOMPRESSED_R16G16B16A16,  // 16*4 bpp (4 channels - half float)
    PIXEL_FORMAT_COMPRESSED_DXT1_RGB,        // 4 bpp (no alpha)
    PIXEL_FORMAT_COMPRESSED_DXT1_RGBA,       // 4 bpp (1 bit alpha)
    PIXEL_FORMAT_COMPRESSED_DXT3_RGBA,       // 8 bpp
    PIXEL_FORMAT_COMPRESSED_DXT5_RGBA,       // 8 bpp
    PIXEL_FORMAT_COMPRESSED_ETC1_RGB,        // 4 bpp
    PIXEL_FORMAT_COMPRESSED_ETC2_RGB,        // 4 bpp
    PIXEL_FORMAT_COMPRESSED_ETC2_EAC_RGBA,   // 8 bpp
    PIXEL_FORMAT_COMPRESSED_PVRT_RGB,        // 4 bpp
    PIXEL_FORMAT_COMPRESSED_PVRT_RGBA,       // 4 bpp
    PIXEL_FORMAT_COMPRESSED_ASTC_4x4_RGBA,   // 8 bpp
    PIXEL_FORMAT_COMPRESSED_ASTC_8x8_RGBA    // 2 bpp
} PixelFormat;

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