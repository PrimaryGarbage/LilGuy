#include "window/window.h"
#include "window/graphics/graphics.h"
#include "random.h"
#include "result.h"
#include "timer.h"
#include "logging.h"
#include "screen_capture.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main(int argc, char* argv[])
{
    RandomInit();
    Timer globalTimer = Timer_Create();
    double deltatime;

    u32 screenCaptureWidth;
    u32 screenCaptureHeight;
    u32* screenCaptureData;
    Result captureResult = CaptureScreen(&screenCaptureWidth, &screenCaptureHeight, &screenCaptureData);
    IF_ERROR_PANIC_EX(captureResult, LogError(&captureResult, NULL););
    Image screenCaptureImage = {
        .width = screenCaptureWidth,
        .height = screenCaptureHeight,
        .format = PIXEL_FORMAT_BGRA,
        .data = screenCaptureData,
    };

    WindowHandle windowHandle;
    Result windowInitResult = Window_Init("LilGuy", WINDOW_WIDTH, WINDOW_HEIGHT, &windowHandle);
    IF_ERROR_PANIC_EX(windowInitResult, LogError(&windowInitResult, NULL););

    Graphics_SetWindow(windowHandle);

    while(Window_Update(windowHandle))
    {
        Window_WaitSync(windowHandle);

        //Graphics_ClearWindow(COLOR_BLACK);
        Graphics_ClearWindowWithImage(&screenCaptureImage);

        // draw here

        deltatime = Timer_Reset(&globalTimer);
    }

    Window_Destroy(windowHandle);
    free(screenCaptureImage.data);

    return 0;
}