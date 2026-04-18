#ifndef __COLOR_H__
#define __COLOR_H__

#include "short_types.h"
#include <assert.h>

//#define COLOR_SCHEME_BGRA
#define COLOR_SCHEME_RGBA


#define COLOR_NOCOLOR   (Color){ 0, 0, 0, 0 }
#define COLOR_WHITE     (Color){ 255, 255, 255, 255 }
#define COLOR_BLACK     (Color){ 0, 0, 0, 255 }
#define COLOR_RED       (Color){ 255, 0, 0, 255 }
#define COLOR_GREEN     (Color){ 0, 255, 0, 255 }
#define COLOR_BLUE      (Color){ 0, 0, 255, 255 }
#define COLOR_YELLOW    (Color){ 255, 255, 0, 255 }
#define COLOR_PURPLE    (Color){ 255, 0, 255, 255 }
#define COLOR_BROWN     (Color){ 255, 255, 0, 255 }

typedef struct Color {
    u8 r;
    u8 g;
    u8 b;
    u8 a;
} Color;

inline static Color Color_New(u8 r, u8 g, u8 b, u8 a)
{
    return (Color){ .r = r, .g = g, .b = b, .a = a };
}


inline static u8 Color_ExtractA(u32 color)
{
    #ifdef COLOR_SCHEME_BGRA
        return (u8)(color >> 24);
    #elifdef COLOR_SCHEME_RGBA
        return (u8)(color >> 24);
    #else // Assume RGBA
        return (u8)(color >> 24);
    #endif
}

inline static u8 Color_ExtractR(u32 color)
{
    #ifdef COLOR_SCHEME_BGRA
        return (u8)((color & 0x00FF0000) >> 16);
    #elifdef COLOR_SCHEME_RGBA
        return (u8)(color & 0x000000FF);
    #else // Assume RGBA
        return (u8)(color & 0x000000FF);
    #endif
}

inline static u8 Color_ExtractG(u32 color)
{
    #ifdef COLOR_SCHEME_BGRA
        return (u8)((color & 0x0000FF00) >> 8);
    #elifdef COLOR_SCHEME_RGBA
        return (u8)((color & 0x0000FF00) >> 8);
    #else // Assume RGBA
        return (u8)((color & 0x0000FF00) >> 8);
    #endif
}

inline static u8 Color_ExtractB(u32 color)
{
    #ifdef COLOR_SCHEME_BGRA
        return (u8)(color & 0x000000FF);
    #elifdef COLOR_SCHEME_RGBA
        return (u8)((color & 0x00FF0000) >> 16);
    #else // Assume RGBA
        return (u8)((color & 0x00FF0000) >> 16);
    #endif
}

inline static u32 Color_Pack(Color color)
{
    #ifdef COLOR_SCHEME_BGRA
        return (u32)(color.a) << 24 | (u32)(color.r) << 16 | (u32)(color.g) << 8 | (u32)(color.b);
    #elifdef COLOR_SCHEME_RGBA
        return (u32)(color.a) << 24 | (u32)(color.b) << 16 | (u32)(color.g) << 8 | (u32)(color.r);
    #else // Assume RGBA
        (u32)(color.a) << 24 | (u32)(color.b) << 16 | (u32)(color.g) << 8 | (u32)(color.r);
    #endif
}

inline static Color Color_Unpack(u32 color)
{
    return (Color){
        .a = Color_ExtractA(color),
        .r = Color_ExtractR(color),
        .g = Color_ExtractG(color),
        .b = Color_ExtractB(color)
    };
}

inline static Color Color_FromHex(u32 hex)
{
    return Color_Unpack(hex);
}

inline static Color Color_Blend(Color fg, Color bg)
{
    assert(0 && "Not implemented");

    if (fg.a == 1.0f) return fg;

    float outA = fg.a + bg.a * (1.0f - fg.a);
    if (outA == 0.0f) return (Color){ 0.0f, 0.0f , 0.0f , 0.0f }; 

    return (Color) {
        (fg.a * fg.r + bg.a * (1.0f - fg.a) * bg.r) / outA,
        (fg.a * fg.g + bg.a * (1.0f - fg.a) * bg.g) / outA,
        (fg.a * fg.b + bg.a * (1.0f - fg.a) * bg.b) / outA,
        outA
    };
}

inline static u32 Color_SwapRAndB(u32 color)
{
    u8* channelPtr = (u8*)&color;
    u8 temp = *(channelPtr);
    *(channelPtr) = *(channelPtr + 2);
    *(channelPtr + 2) = temp;

    return color;
}

inline static Color Color_Lerp(const Color* a, const Color* b, float weight)
{
    return (Color){ 
        .r = a->r + (b->r - a->r) * weight,
        .g = a->g + (b->g - a->g) * weight,
        .b = a->b + (b->b - a->b) * weight,
        .a = a->a + (b->a - a->a) * weight,
    };
}

#endif // __COLOR_H__