#include "graphics.h"
#include "MiniFB.h"
#include "graphics_context.h"
#include "logging.h"
#include "rect.h"
#include "math_helpers.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include "result.h"
#include "screen_capture.h"
#include "stb_image_write.h"

#define GRAPHICS_CONTEXT_COUNT_MAX 4

static GraphicsContext* Context;
static bool ContextInitialized;


static inline Vector2u RasterizePoint(float x, float y)
{
    return (Vector2u){
        roundf(Clampf(x, 0.0f, Context->windowSize.x - 1)),
        roundf(Clampf(y, 0.0f, Context->windowSize.y - 1))
    };
}

static inline Vector2u RasterizePointV(Vector2 pos)
{
    return RasterizePoint(pos.x, pos.y);
}

static inline void PutPixel(u32 x, u32 y, Color color)
{
    u32 idx = x + Context->windowSize.x * y;
    Color bgColor = Color_Unpack(Context->buffer[idx]);
    Context->buffer[idx] = Color_Pack(Color_Blend(color, bgColor));
}

static inline void PutPixelV(Vector2u pos, Color color)
{
    PutPixel(pos.x, pos.y, color);
}

void WindowMouseCallback(mfb_window* window, int x, int y)
{
    Context->mousePosition.x = Clampi(x, 0, Context->windowSize.x) + 0.5f;
    Context->mousePosition.y = Clampi(y, 0, Context->windowSize.y) + 0.5f;
}

Result Graphics_Init(const char* windowTitle, u32 windowWidth, u32 windowHeight)
{
    Context = malloc(sizeof(GraphicsContext));

    u32 screenCaptureWidth;
    u32 screenCaptureHeight;
    Result captureScreenResult = CaptureScreen(&screenCaptureWidth, &screenCaptureHeight);
    IF_ERROR_RETURN(captureScreenResult);
    u32* screenCaptureData = captureScreenResult.value;

    Context->screenCapture = (Image){
        screenCaptureWidth,
        screenCaptureHeight,
        PIXEL_FORMAT_BGRA,
        screenCaptureData
    };

    struct mfb_window* window = mfb_open_ex("LilGuy", windowWidth, windowHeight, 0);
    if (window == NULL)
        return Error(RESULT_MINIFB_INITIALIZATION_ERROR, "Failed to initialize MiniFB window");

    Context->window = window;

    mfb_set_mouse_move_callback(window, WindowMouseCallback);

    Context->bufferSize = windowWidth * windowHeight * sizeof(u32);
    Context->windowSize = (Vector2u){ windowWidth, windowHeight };
    Context->buffer = malloc(Context->bufferSize);

    ContextInitialized = true;

    return Success(NULL);
}

bool Graphics_Update()
{
    if (!ContextInitialized)
    {
        LogErrorM("Using graphics functions without initializing context is forbidden");
        exit(1);
    }

    Context->windowUpdateState = mfb_update_ex(Context->window, Context->buffer, Context->windowSize.x, Context->windowSize.y);

    if (Context->windowUpdateState != MFB_STATE_OK)
    {
        if (Context->windowUpdateState != MFB_STATE_EXIT)
        {
            LogErrorM("Window state returned '%d'", Context->windowUpdateState);
            exit(1);
        }

        return false;
    }

    return true;
}

void Graphics_Deinit()
{
    if (!ContextInitialized)
    {
        LogErrorM("Using graphics functions without initializing context is forbidden");
        exit(1);
    }

    free(Context->buffer);
    Image_Free(&Context->screenCapture);
    ContextInitialized = false;
}

void Graphics_ClearWindow(Color color)
{
    u32 packedColor = Color_Pack(color);

    for(size_t i = 0u; i < Context->bufferSize / sizeof(u32); ++i)
        Context->buffer[i] = packedColor;
}

void Graphics_ClearWindowWithImage(const Image* image)
{
    Graphics_DrawImage(image, Graphics_GetWindowRect());
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
        .a = { roundf(Clampf(position.x - radius - 0.5f, 0, Context->windowSize.x - 1)), roundf(Clampf(position.y - radius - 0.5f, 0, Context->windowSize.y - 1)) },
        .b = { roundf(Clampf(position.x + radius + 0.5f, 0, Context->windowSize.x - 1)), roundf(Clampf(position.y + radius + 0.5f, 0, Context->windowSize.y - 1)) },
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
    a.x = Clampf(a.x, 0.0f, Context->windowSize.x - 1);
    a.y = Clampf(a.y, 0.0f, Context->windowSize.y - 1);
    b.x = Clampf(b.x, 0.0f, Context->windowSize.x - 1);
    b.y = Clampf(b.y, 0.0f, Context->windowSize.y - 1);

    float dx = b.x - a.x;
    float dy = b.y - a.y;
    float steps = MAX(fabsf(dx), fabsf(dy));
    float xInc = dx / steps;
    float yInc = dy / steps;

    for(u32 i = 0; i < steps; ++i)
        PutPixelV(RasterizePoint(a.x + xInc * i, a.y + yInc * i), color);
}

void Graphics_WaitSync()
{
    if (Context->windowUpdateState == MFB_STATE_OK)
        while(!mfb_wait_sync(Context->window));
}

Vector2 Graphics_GetMousePosition()
{
    return Context->mousePosition;
}

Image* Graphics_GetScreenCaptureImage()
{
    return &Context->screenCapture;
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

void Graphics_WriteImageToPngFile(const Image* image, const char* filepath)
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

            for (size_t i = 0; i < pixelCount; ++i)
                tempBuffer[i] = Color_SwapRAndB(image->data[i]);

            stbi_write_png(filepath, image->width, image->height, 4, tempBuffer, image->width * 4);
            free(tempBuffer);
            break;
        }
    }
}

Rect Graphics_GetWindowRect()
{
    return (Rect) {
        .a = { 0.0f, 0.0f, },
        .b = { Context->windowSize.x, Context->windowSize.y }
    };
}