#include "window.h"
#include "window_context.h"
#include <math.h>
#include "result.h"
#include "raylib_wrapper.h"

static Image BufferImage;

Result Window_Init(const char* windowTitle, u32 windowWidth, u32 windowHeight, WindowHandle* handle_out)
{
    WindowContext* context = malloc(sizeof(WindowContext));
    if (context == NULL) MEMORY_PANIC();

    u32 screenCaptureWidth;
    u32 screenCaptureHeight;
    u32* screenCaptureData;

    Raylib_InitWindow(windowWidth, windowHeight, windowTitle);

    context->bufferSize = windowWidth * windowHeight * sizeof(u32);
    context->windowSize = (Vector2u){ windowWidth, windowHeight };
    context->buffer = malloc(context->bufferSize);
    context->shouldClose = false;

    *handle_out = context;
    
    BufferImage = (Image) {
        .data = context->buffer,
        .width = context->windowSize.x,
        .height = context->windowSize.y,
        .format = PIXEL_FORMAT_RGBA,
        .dataSize = context->bufferSize
    };

    return Success();
}

void Window_DrawBuffer(WindowHandle handle)
{
    Texture2D bufferTexture = Raylib_LoadTextureFromImage(&BufferImage);
    Rect destRect = {{0.0f, 0.0f}, {handle->windowSize.x, handle->windowSize.y}};
    Raylib_DrawTexturePro(bufferTexture, destRect, destRect, Vector2_Zero(), 0.0f, COLOR_WHITE);
    Raylib_rlDrawRenderBatchActive();
    Raylib_SwapBuffers();
    Raylib_UnloadTexture(bufferTexture);
}

void Window_PollEvents(WindowHandle handle)
{
    Raylib_PollInputEvents();
}

void Window_Destroy(WindowHandle handle)
{
    free(handle->buffer);
    free(handle);
}

Rect Window_GetWindowRect(WindowHandle handle)
{
    return (Rect) {
        .a = { 0.0f, 0.0f, },
        .b = { handle->windowSize.x, handle->windowSize.y }
    };
}

bool Window_ShouldClose(WindowHandle handle)
{
    return Raylib_WindowShouldClose();
}