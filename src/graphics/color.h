#ifndef __COLOR_H__
#define __COLOR_H__

#include "short_types.h"

#define COLOR_SCHEME_BGRA
//#define COLOR_SCHEME_RGBA

#define COLOR_WHITE   (Color){ 1.0f, 1.0f, 1.0f, 1.0f }
#define COLOR_BLACK   (Color){ 0.0f, 0.0f, 0.0f, 1.0f }
#define COLOR_RED     (Color){ 1.0f, 0.0f, 0.0f, 1.0f }
#define COLOR_GREEN   (Color){ 0.0f, 1.0f, 0.0f, 1.0f }
#define COLOR_BLUE    (Color){ 0.0f, 0.0f, 1.0f, 1.0f }
#define COLOR_YELLOW  (Color){ 1.0f, 1.0f, 0.0f, 1.0f }
#define COLOR_PURPLE (Color){ 1.0f, 0.0f, 1.0f, 1.0f }
#define COLOR_BROWN (Color){ 1.0f, 1.0f, 0.0f, 1.0f }

typedef struct Color {
    float r;
    float g;
    float b;
    float a;
} Color;

inline static u8 Color_ExtractA(u32 color)
{
    #ifdef COLOR_SCHEME_BGRA
        return (u8)(color >> 24);
    #elifdef COLOR_SCHEME_RGBA
        return (u8)(color >> 24)
    #else // Assume RGBA
        return (u8)(color >> 24)
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
        return ((u32)(color.a * 255.0f)) << 24 | ((u32)(color.r * 255.0f)) << 16 | ((u32)(color.g * 255.0f)) << 8 | (u32)(color.b * 255.0f);
    #elifdef COLOR_SCHEME_RGBA
        return ((u32)(color.a * 255.0f)) << 24 | ((u32)(color.b * 255.0f)) << 16 | ((u32)(color.g * 255.0f)) << 8 | (u32)(color.r * 255.0f);
    #else // Assume RGBA
        ((u32)(color.a * 255.0f)) << 24 | ((u32)(color.b * 255.0f)) << 16 | ((u32)(color.g * 255.0f)) << 8 | (u32)(color.r * 255.0f);
    #endif
}

inline static Color Color_Unpack(u32 color)
{
    return (Color){
        .a = Color_ExtractA(color) / 255.0f,
        .r = Color_ExtractR(color) / 255.0f,
        .g = Color_ExtractG(color) / 255.0f,
        .b = Color_ExtractB(color) / 255.0f
    };
}

inline static Color Color_Blend(Color fg, Color bg)
{
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

#endif // __COLOR_H__