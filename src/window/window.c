#include "window.h"
#include "MiniFB.h"
#include "MiniFB_enums.h"
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

    struct mfb_window* window = mfb_open_ex("LilGuy", windowWidth, windowHeight, MFB_WF_ALWAYS_ON_TOP | MFB_WF_BORDERLESS | MFB_WF_FULLSCREEN);
    if (window == NULL)
        return Error(RESULT_MINIFB_INITIALIZATION_ERROR, "Failed to initialize MiniFB window");

    context->mfbWindow = window;

    context->bufferSize = windowWidth * windowHeight * sizeof(u32);
    context->windowSize = (Vector2u){ windowWidth, windowHeight };
    context->buffer = malloc(context->bufferSize);
    context->shouldClose = false;

    *handle_out = context;

    return Success();
}

void Window_Refresh(WindowHandle handle)
{
    handle->windowUpdateState = mfb_update(handle->mfbWindow, handle->buffer);

    if (handle->windowUpdateState != MFB_STATE_OK)
    {
        handle->shouldClose = true;

        if (handle->windowUpdateState != MFB_STATE_EXIT)
        {
            PANIC_EX(LogErrorM("Window state returned '%d'", handle->windowUpdateState););
        }
    }
}

void Window_PollEvents(WindowHandle handle)
{
    mfb_update_events(handle->mfbWindow);
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

Rect Window_GetWindowRect(WindowHandle handle)
{
    return (Rect) {
        .a = { 0.0f, 0.0f, },
        .b = { handle->windowSize.x, handle->windowSize.y }
    };
}

bool Window_ShouldClose(WindowHandle handle)
{
    return handle->shouldClose;
}