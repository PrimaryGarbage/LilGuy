#include "graphics/color.h"
#include "graphics/image.h"
#include "input/input.h"
#include "input/input_button.h"
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
        .format = PIXEL_FORMAT_RGBA,
        .data = screenCaptureData,
        .dataSize = screenCaptureWidth * screenCaptureHeight * 4
    };
    Image_SwapRAndBChannels(&screenCaptureImage);


    u32 windowWidth = screenCaptureWidth;
    u32 windowHeight = screenCaptureHeight;

    RandomInit();
    Timer globalTimer = Timer_Create();
    double deltatime;

    WindowHandle windowHandle;
    Result windowInitResult = Window_Init("LilGuy", windowWidth, windowHeight, &windowHandle);
    IF_ERROR_PANIC_EX(windowInitResult, LogError(&windowInitResult, NULL););

    Graphics_SetWindow(windowHandle);

    Texture2D screenCaptureTexture = Graphics_LoadTextureFromImage(&screenCaptureImage);
    Graphics_DrawTextureFullscreen(&screenCaptureTexture);

    Scene* mainCharScene = MainCharScene_Create();

    while(!Window_ShouldClose(windowHandle))
    {
        Window_PollEvents(windowHandle);

        ///////////////////
        /// UPDATE HERE ///
        Scene_Update(mainCharScene, deltatime);
        ///////////////////

        Graphics_ClearWindow(COLOR_NOCOLOR);
        Graphics_DrawTextureFullscreen(&screenCaptureTexture);

        Graphics_DrawSquare((Vector2){ -100.0f, 0.0f }, 100.0f, COLOR_RED, false);

        if (Input_IsKeyPressed(INPUT_KEY_ESCAPE))
            break;

        /////////////////
        /// DRAW HERE ///
        Scene_Draw(mainCharScene);
        /////////////////

        Window_DrawBuffer(windowHandle);

        deltatime = Timer_Reset(&globalTimer);
    }

    Scene_Destroy(mainCharScene);

    free(screenCaptureImage.data);
    Window_Destroy(windowHandle);

    return 0;
}