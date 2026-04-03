#include "graphics.h"
#include "../window_context.h"
#include "math_helpers.h"
#include "../window.h"
#include "stb_image_write.h"
#include <math.h>
#include <stdlib.h>

static WindowHandle Window;

static inline Vector2u RasterizePoint(float x, float y)
{
    return (Vector2u){
        roundf(Clampf(x, 0.0f, Window->windowSize.x - 1)),
        roundf(Clampf(y, 0.0f, Window->windowSize.y - 1))
    };
}

static inline Vector2u RasterizePointV(Vector2 pos)
{
    return RasterizePoint(pos.x, pos.y);
}

static inline void PutPixel(u32 x, u32 y, Color color)
{
    u32 idx = x + Window->windowSize.x * y;
    Color bgColor = Color_Unpack(Window->buffer[idx]);
    Window->buffer[idx] = Color_Pack(Color_Blend(color, bgColor));
}

static inline void PutPixelV(Vector2u pos, Color color)
{
    PutPixel(pos.x, pos.y, color);
}

void Graphics_SetWindow(WindowHandle handle)
{
    Window = handle;
}

void Graphics_DrawRect(Rect rect, Color color, bool wireframe)
{
    Rect_u onScreenRect = {
        .a = RasterizePointV(rect.a),
        .b = RasterizePointV(rect.b)
    };

    if (wireframe)
    {
        for (u32 i = onScreenRect.a.x; i <= onScreenRect.b.x; ++i)
        {
            PutPixel(i, onScreenRect.a.y, color);
            PutPixel(i, onScreenRect.b.y, color);
        }

        for (u32 j = onScreenRect.a.y; j <= onScreenRect.b.y; ++j)
        {
            PutPixel(onScreenRect.a.x, j, color);
            PutPixel(onScreenRect.b.x, j, color);
        }
    }
    else
    {
        for (u32 i = onScreenRect.a.x; i <= onScreenRect.b.x; ++i)
        {
            for (u32 j = onScreenRect.a.y; j <= onScreenRect.b.y; ++j)
            {
                PutPixel(i, j, color);
            }
        }
    }
}

void Graphics_DrawSquare(Vector2 position, float size, Color color, bool wireframe)
{
    Graphics_DrawRect((Rect){ .a = position, .b = Vector2_AddScalar(position, size)}, color, wireframe);
}

void Graphics_DrawCircle(Vector2 position, float radius, Color color, bool wireframe)
{
    if (radius < 0)
        radius = -radius;

    Rect_u onScreenRect = {
        .a = { roundf(Clampf(position.x - radius - 0.5f, 0, Window->windowSize.x - 1)), roundf(Clampf(position.y - radius - 0.5f, 0, Window->windowSize.y - 1)) },
        .b = { roundf(Clampf(position.x + radius + 0.5f, 0, Window->windowSize.x - 1)), roundf(Clampf(position.y + radius + 0.5f, 0, Window->windowSize.y - 1)) },
    };

    float radiusSquared = SQUARED(radius);
    float radiusSquaredWireframeDelta = sqrt(radiusSquared * 0.8);

    for (u32 i = onScreenRect.a.x; i < onScreenRect.b.x; ++i)
    {
        for (u32 j = onScreenRect.a.y; j < onScreenRect.b.y; ++j)
        {
            if (wireframe)
            {
                float hypotenuse = SQUARED(position.x - (i32)i) + SQUARED(position.y - (i32)j);
                if (fabsf(hypotenuse - (i32)radiusSquared) < radiusSquaredWireframeDelta)
                    PutPixel(i, j, color);
            }
            else
            {
                if (SQUARED((i32)position.x - (i32)i) + SQUARED((i32)position.y - (i32)j) <= radiusSquared)
                    PutPixel(i, j, color);
            }
        }
    }
}

void Graphics_DrawLine(Vector2 a, Vector2 b, float width, Color color)
{
    a.x = Clampf(a.x, 0.0f, Window->windowSize.x - 1);
    a.y = Clampf(a.y, 0.0f, Window->windowSize.y - 1);
    b.x = Clampf(b.x, 0.0f, Window->windowSize.x - 1);
    b.y = Clampf(b.y, 0.0f, Window->windowSize.y - 1);

    float dx = b.x - a.x;
    float dy = b.y - a.y;
    float steps = MAX(fabsf(dx), fabsf(dy));
    float xInc = dx / steps;
    float yInc = dy / steps;

    for(u32 i = 0; i < steps; ++i)
        PutPixelV(RasterizePoint(a.x + xInc * i, a.y + yInc * i), color);
}

void Graphics_DrawImage(const Image* image, Rect rect)
{
    Rect_u onScreenRect = {
        .a = RasterizePointV(rect.a),
        .b = RasterizePointV(rect.b)
    };

    float rectWidth = rect.b.x - rect.a.x;
    float rectHeight = rect.b.y - rect.a.y;

    if (rectWidth <= 0.0f || rectHeight <= 0.0f)
        return;

    for (u32 i = onScreenRect.a.x; i <= onScreenRect.b.x; ++i)
    {
        for (u32 j = onScreenRect.a.y; j <= onScreenRect.b.y; ++j)
        {
            float imageXNormalized = ((float)(i - onScreenRect.a.x)) / rectWidth;
            float imageYNormalized = ((float)(j - onScreenRect.a.y)) / rectHeight;
            PutPixel(i, j, Image_SampleNormalized(image, imageXNormalized, imageYNormalized));
        }
    }
}

void Window_WriteImageToPngFile(WindowHandle handle, const Image* image, const char* filepath)
{
    switch(image->format)
    {
        case PIXEL_FORMAT_RGBA:
        {
            stbi_write_png(filepath, image->width, image->height, 4, image->data, image->width * 4);
            break;
        }
        case PIXEL_FORMAT_BGRA:
        {
            size_t pixelCount = image->width * image->height;
            u32* tempBuffer = malloc(pixelCount * 4);
            if(!tempBuffer) MEMORY_PANIC();

            for (size_t i = 0; i < pixelCount; ++i)
                tempBuffer[i] = Color_SwapRAndB(image->data[i]);

            stbi_write_png(filepath, image->width, image->height, 4, tempBuffer, image->width * 4);
            free(tempBuffer);
            break;
        }
    }
}

void Graphics_ClearWindow(Color color)
{
    u32 packedColor = Color_Pack(color);

    for(size_t i = 0u; i < Window->bufferSize / sizeof(u32); ++i)
        Window->buffer[i] = packedColor;
}

void Graphics_ClearWindowWithImage(const Image* image)
{
    Graphics_DrawImage(image, Window_GetWindowRect(Window));
}

Image* Graphics_GetScreenCaptureImage()
{
    return &Window->screenCapture;
}