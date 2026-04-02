#include "graphics.h"
#include "graphics_context.h"
#include "memory_s.h"
#include "logging.h"
#include "rect.h"
#include "math_helpers.h"
#include <string.h>
#include <math.h>
#include "screen_capture.h"

#define GRAPHICS_CONTEXT_COUNT_MAX 4

static GraphicsContext* Context;
static bool ContextInitialized;


static inline Vector2_u RasterizePoint(float x, float y)
{
    return (Vector2_u){
        roundf(Clampf(x, 0.0f, Context->windowSize.x - 1)),
        roundf(Clampf(y, 0.0f, Context->windowSize.y - 1))
    };
}

static inline Vector2_u RasterizePointV(Vector2 pos)
{
    return RasterizePoint(pos.x, pos.y);
}

static inline void PutPixel(u32 x, u32 y, Color color)
{
    u32 idx = x + Context->windowSize.x * y;
    u32 packedBg = Context->buffer[idx];
    Color bgColor = UnpackColor(Context->buffer[idx]);
    Context->buffer[idx] = PackColor(BlendColors(color, bgColor));
}

static inline void PutPixelV(Vector2_u pos, Color color)
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
    Context = malloc_s(sizeof(GraphicsContext));

    u32 screenCaptureWidth;
    u32 screenCaptureHeight;
    u32* screenCaptureData = CaptureScreen(&screenCaptureWidth, &screenCaptureHeight);
    Context->screenCapture = (Image){
        screenCaptureWidth,
        screenCaptureHeight,
        screenCaptureData
    };

    struct mfb_window* window = mfb_open_ex("LilGuy", windowWidth, windowHeight, 0);
    if (window == NULL)
        return Error(RESULT_MINIFB_INITIALIZATION_ERROR, "Failed to initialize MiniFB window");

    Context->window = window;

    mfb_set_mouse_move_callback(window, WindowMouseCallback);

    Context->bufferSize = windowWidth * windowHeight * sizeof(u32);
    Context->windowSize = (Vector2_u){ windowWidth, windowHeight };
    Context->buffer = malloc_s(Context->bufferSize);

    ContextInitialized = true;

    return result_success;
}

bool Graphics_Update()
{
    if (!ContextInitialized)
    {
        LogErrorM("Using graphics functions without initializing context is forbidden");
        exit(1);
    }

    mfb_wait_sync(Context->window);
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
    u32 packedColor = PackColor(color);

    for(size_t i = 0u; i < Context->bufferSize / sizeof(u32); ++i)
        Context->buffer[i] = packedColor;
}

void Graphics_DrawRect(Rect rect, Color color, bool wireframe)
{
    Rect_u aabb = {
        .a = RasterizePointV(rect.a),
        .b = RasterizePointV(rect.b)
    };

    if (wireframe)
    {
        for (u32 i = aabb.a.x; i <= aabb.b.x; ++i)
        {
            PutPixel(i, aabb.a.y, color);
            PutPixel(i, aabb.b.y, color);
        }

        for (u32 j = aabb.a.y; j <= aabb.b.y; ++j)
        {
            PutPixel(aabb.a.x, j, color);
            PutPixel(aabb.b.x, j, color);
        }
    }
    else
    {
        for (u32 i = aabb.a.x; i <= aabb.b.x; ++i)
        {
            for (u32 j = aabb.a.y; j <= aabb.b.y; ++j)
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

    Rect_u aabb = {
        .a = { roundf(Clampf(position.x - radius - 0.5f, 0, Context->windowSize.x - 1)), roundf(Clampf(position.y - radius - 0.5f, 0, Context->windowSize.y - 1)) },
        .b = { roundf(Clampf(position.x + radius + 0.5f, 0, Context->windowSize.x - 1)), roundf(Clampf(position.y + radius + 0.5f, 0, Context->windowSize.y - 1)) },
    };

    float radiusSquared = SQUARED(radius);
    float radiusSquaredWireframeDelta = sqrt(radiusSquared * 0.8);

    for (u32 i = aabb.a.x; i < aabb.b.x; ++i)
    {
        for (u32 j = aabb.a.y; j < aabb.b.y; ++j)
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

Vector2 Graphics_GetMousePosition()
{
    return Context->mousePosition;
}