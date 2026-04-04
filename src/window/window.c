#include "window.h"
#include "MiniFB.h"
#include "window_callbacks.h"
#include "window_context.h"
#include "logging.h"
#include <math.h>
#include "result.h"
#include "screen_capture.h"
#include "stb_image_write.h"

Result Window_Init(const char* windowTitle, u32 windowWidth, u32 windowHeight, WindowHandle* handle_out)
{
    WindowContext* context = malloc(sizeof(WindowContext));
    if (context == NULL) MEMORY_PANIC();

    u32 screenCaptureWidth;
    u32 screenCaptureHeight;
    u32* screenCaptureData;
    Result captureScreenResult = CaptureScreen(&screenCaptureWidth, &screenCaptureHeight, &screenCaptureData);
    IF_ERROR_RETURN(captureScreenResult);

    context->screenCapture = (Image){
        screenCaptureWidth,
        screenCaptureHeight,
        PIXEL_FORMAT_BGRA,
        screenCaptureData
    };

    struct mfb_window* window = mfb_open_ex("LilGuy", windowWidth, windowHeight, 0);
    if (window == NULL)
        return Error(RESULT_MINIFB_INITIALIZATION_ERROR, "Failed to initialize MiniFB window");

    context->mfbWindow = window;

    context->bufferSize = windowWidth * windowHeight * sizeof(u32);
    context->windowSize = (Vector2u){ windowWidth, windowHeight };
    context->buffer = malloc(context->bufferSize);
    context->onUpdateCallbacksCount = 0u;

    *handle_out = context;

    return Success();
}

bool Window_Update(WindowHandle handle)
{
    for(u32 i = 0u; i < handle->onUpdateCallbacksCount; ++i)
        handle->onUpdateCallbacks[i]();

    handle->windowUpdateState = mfb_update_ex(handle->mfbWindow, handle->buffer, handle->windowSize.x, handle->windowSize.y);

    if (handle->windowUpdateState != MFB_STATE_OK)
    {
        if (handle->windowUpdateState != MFB_STATE_EXIT)
        {
            PANIC_EX(LogErrorM("Window state returned '%d'", handle->windowUpdateState););
        }

        return false;
    }

    return true;
}

void Window_Destroy(WindowHandle handle)
{
    free(handle->buffer);
    Image_Free(&handle->screenCapture);
    free(handle);
}

void Window_WaitSync(WindowHandle handle)
{
    if (handle->windowUpdateState == MFB_STATE_OK)
        while(!mfb_wait_sync(handle->mfbWindow));
}

void Window_AddOnUpdateCallback(WindowHandle handle, Window_OnUpdateCallback callback)
{
    if (handle->onUpdateCallbacksCount == WINDOW_ON_UPDATE_CALLBACK_MAX)
        PANIC_EX(LogErrorM("Max number of OnUpdateCallbacks exceeded"););

    handle->onUpdateCallbacks[handle->onUpdateCallbacksCount++] = callback;
}

Rect Window_GetWindowRect(WindowHandle handle)
{
    return (Rect) {
        .a = { 0.0f, 0.0f, },
        .b = { handle->windowSize.x, handle->windowSize.y }
    };
}