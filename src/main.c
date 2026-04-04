#include "graphics/image.h"
#include "input/input.h"
#include "scene/scene.h"
#include "window/window.h"
#include "graphics/graphics.h"
#include "random.h"
#include "result.h"
#include "timer.h"
#include "logging.h"
#include "screen_capture.h"
#include "scene/main_char_scene.h"

int main(int argc, char* argv[])
{
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
        .dataSize = screenCaptureWidth * screenCaptureHeight * 4
    };

    u32 windowWidth = screenCaptureWidth;
    u32 windowHeight = screenCaptureHeight;

    RandomInit();
    Timer globalTimer = Timer_Create();
    double deltatime;

    WindowHandle windowHandle;
    Result windowInitResult = Window_Init("LilGuy", windowWidth, windowHeight, &windowHandle);
    IF_ERROR_PANIC_EX(windowInitResult, LogError(&windowInitResult, NULL););

    Graphics_SetWindow(windowHandle);
    Graphics_ClearWindowWithImage(&screenCaptureImage);

    Input_SetWindow(windowHandle);

    Scene* mainCharScene = MainCharScene_Create();

    while(Window_Update(windowHandle))
    {
        Window_WaitSync(windowHandle);

        SCENE_UPDATE(mainCharScene, deltatime);

        Graphics_ClearWindowWithImage(&screenCaptureImage);

        if (Input_IsButtonPressed(INPUT_KB_KEY_ESCAPE))
            break;

        /////////////////
        /// DRAW HERE ///
        /////////////////

        SCENE_DRAW(mainCharScene);

        deltatime = Timer_Reset(&globalTimer);
    }

    Scene_Destroy(mainCharScene);

    free(screenCaptureImage.data);
    Window_Destroy(windowHandle);

    return 0;
}