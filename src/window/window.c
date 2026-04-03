#include "window.h"
#include "MiniFB.h"
#include "window_context.h"
#include "logging.h"
#include <math.h>
#include "result.h"
#include "screen_capture.h"
#include "stb_image_write.h"

void WindowMouseCallback(mfb_window* window, int x, int y)
{
    //Context->mousePosition.x = Clampi(x, 0, Context->windowSize.x) + 0.5f;
    //Context->mousePosition.y = Clampi(y, 0, Context->windowSize.y) + 0.5f;
}

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

    mfb_set_mouse_move_callback(window, WindowMouseCallback);

    context->bufferSize = windowWidth * windowHeight * sizeof(u32);
    context->windowSize = (Vector2u){ windowWidth, windowHeight };
    context->buffer = malloc(context->bufferSize);

    *handle_out = context;

    return Success();
}

bool Window_Update(WindowHandle handle)
{
    handle->windowUpdateState = mfb_update_ex(handle->mfbWindow, handle->buffer, handle->windowSize.x, handle->windowSize.y);

    if (handle->windowUpdateState != MFB_STATE_OK)
    {
        if (handle->windowUpdateState != MFB_STATE_EXIT)
        {
            LogErrorM("Window state returned '%d'", handle->windowUpdateState);
            exit(1);
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

Vector2 Window_GetMousePosition(WindowHandle handle)
{
    return handle->mousePosition;
}

Rect Window_GetWindowRect(WindowHandle handle)
{
    return (Rect) {
        .a = { 0.0f, 0.0f, },
        .b = { handle->windowSize.x, handle->windowSize.y }
    };
}