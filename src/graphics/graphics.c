#include "graphics.h"
#include "graphics_context.h"
#include "memory_s.h"
#include "logging.h"
#include "rect.h"
#include "math_helpers.h"
#include <string.h>
#include <math.h>

#define GRAPHICS_CONTEXT_COUNT_MAX 4

static GraphicsContext* Context;
static bool ContextInitialized;


static inline Vector2_u rasterizePoint(float x, float y)
{
    return (Vector2_u){
        roundf(clamp_f(x, 0.0f, Context->windowSize.x - 1)),
        roundf(clamp_f(y, 0.0f, Context->windowSize.y - 1))
    };
}

static inline Vector2_u rasterizePointV(Vector2 pos)
{
    return rasterizePoint(pos.x, pos.y);
}

static inline void putPixel(u32 x, u32 y, Color color)
{
    u32 idx = x + Context->windowSize.x * y;
    u32 packedBg = Context->buffer[idx];
    Color bgColor = unpackColor(Context->buffer[idx]);
    Context->buffer[idx] = packColor(blendColors(color, bgColor));
}

static inline void putPixelV(Vector2_u pos, Color color)
{
    putPixel(pos.x, pos.y, color);
}

void windowMouseCallback(mfb_window* window, int x, int y)
{
    Context->mousePosition.x = clamp_i(x, 0, Context->windowSize.x) + 0.5f;
    Context->mousePosition.y = clamp_i(y, 0, Context->windowSize.y) + 0.5f;
}

Result graphics_init(const char* windowTitle, u32 windowWidth, u32 windowHeight)
{
    Context = malloc_s(sizeof(GraphicsContext));

    struct mfb_window* window = mfb_open_ex("LilGuy", windowWidth, windowHeight, 0);
    if (window == NULL)
        return error(RESULT_MINIFB_INITIALIZATION_ERROR, "Failed to initialize MiniFB window");

    Context->window = window;

    mfb_set_mouse_move_callback(window, windowMouseCallback);

    Context->bufferSize = windowWidth * windowHeight * sizeof(u32);
    Context->windowSize = (Vector2_u){ windowWidth, windowHeight };
    Context->buffer = malloc_s(Context->bufferSize);

    ContextInitialized = true;

    return result_success;
}

bool graphics_update()
{
    if (!ContextInitialized)
    {
        logErrorM("Using graphics functions without initializing context is forbidden");
        exit(1);
    }

    mfb_wait_sync(Context->window);
    Context->windowUpdateState = mfb_update_ex(Context->window, Context->buffer, Context->windowSize.x, Context->windowSize.y);

    if (Context->windowUpdateState != MFB_STATE_OK)
    {
        if (Context->windowUpdateState != MFB_STATE_EXIT)
        {
            logErrorM("Window state returned '%d'", Context->windowUpdateState);
            exit(1);
        }

        return false;
    }

    return true;
}

void graphics_deinit()
{
    if (!ContextInitialized)
    {
        logErrorM("Using graphics functions without initializing context is forbidden");
        exit(1);
    }

    free(Context->buffer);
    free(Context);
    ContextInitialized = false;
}

void graphics_clearWindow(Color color)
{
    u32 packedColor = packColor(color);

    for(size_t i = 0u; i < Context->bufferSize / sizeof(u32); ++i)
        Context->buffer[i] = packedColor;
}

void graphics_drawRect(Rect rect, Color color, bool wireframe)
{
    Rect_u aabb = {
        .a = rasterizePointV(rect.a),
        .b = rasterizePointV(rect.b)
    };

    if (wireframe)
    {
        for (u32 i = aabb.a.x; i <= aabb.b.x; ++i)
        {
            putPixel(i, aabb.a.y, color);
            putPixel(i, aabb.b.y, color);
        }

        for (u32 j = aabb.a.y; j <= aabb.b.y; ++j)
        {
            putPixel(aabb.a.x, j, color);
            putPixel(aabb.b.x, j, color);
        }
    }
    else
    {
        for (u32 i = aabb.a.x; i <= aabb.b.x; ++i)
        {
            for (u32 j = aabb.a.y; j <= aabb.b.y; ++j)
            {
                putPixel(i, j, color);
            }
        }
    }
}

void graphics_drawSquare(Vector2 position, float size, Color color, bool wireframe)
{
    graphics_drawRect((Rect){ .a = position, .b = Vector2_addScalar(position, size)}, color, wireframe);
}

void graphics_drawCircle(Vector2 position, float radius, Color color, bool wireframe)
{
    if (radius < 0)
        radius = -radius;

    Rect_u aabb = {
        .a = { roundf(clamp_f(position.x - radius - 0.5f, 0, Context->windowSize.x - 1)), roundf(clamp_f(position.y - radius - 0.5f, 0, Context->windowSize.y - 1)) },
        .b = { roundf(clamp_f(position.x + radius + 0.5f, 0, Context->windowSize.x - 1)), roundf(clamp_f(position.y + radius + 0.5f, 0, Context->windowSize.y - 1)) },
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
                    putPixel(i, j, color);
            }
            else
            {
                if (SQUARED((i32)position.x - (i32)i) + SQUARED((i32)position.y - (i32)j) <= radiusSquared)
                    putPixel(i, j, color);
            }
        }
    }
}

void graphics_drawLine(Vector2 a, Vector2 b, float width, Color color)
{
    a.x = clamp_f(a.x, 0.0f, Context->windowSize.x - 1);
    a.y = clamp_f(a.y, 0.0f, Context->windowSize.y - 1);
    b.x = clamp_f(b.x, 0.0f, Context->windowSize.x - 1);
    b.y = clamp_f(b.y, 0.0f, Context->windowSize.y - 1);

    float dx = b.x - a.x;
    float dy = b.y - a.y;
    float steps = MAX(fabsf(dx), fabsf(dy));
    float xInc = dx / steps;
    float yInc = dy / steps;

    for(u32 i = 0; i < steps; ++i)
        putPixelV(rasterizePoint(a.x + xInc * i, a.y + yInc * i), color);
}

Vector2 graphics_getMousePosition()
{
    return Context->mousePosition;
}