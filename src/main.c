#include "graphics/color.h"
#include "graphics/image.h"
#include "input/input.h"
#include "input/input_button.h"
#include "raylib_wrapper.h"
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
    Vector2u windowSize;
    u32* screenCaptureData;
    Result captureResult = CaptureScreen(&windowSize.x, &windowSize.y, &screenCaptureData);
    IF_ERROR_PANIC_EX(captureResult, LogError(&captureResult, NULL););

    Image screenCaptureImage = {
        .width = windowSize.x,
        .height = windowSize.y,
        .format = PIXEL_FORMAT_RGBA,
        .data = screenCaptureData,
        .dataSize = windowSize.x * windowSize.y * 4
    };
    Image_SwapRAndBChannels(&screenCaptureImage);

    RandomInit();
    Timer globalTimer = Timer_Create();
    double deltatime;

    Window_Init("LilGuy", windowSize);
    Window_Hide();

    Texture2D screenCaptureTexture = Graphics_LoadTextureFromImage(&screenCaptureImage);
    Graphics_DrawTextureFullscreen(&screenCaptureTexture);
    Graphics_Flush();
    Window_Show();

    Scene* mainCharScene = MainCharScene_Create();

    while(!Window_ShouldClose())
    {
        Window_PollEvents();

        if (Input_IsKeyPressed(INPUT_KEY_ESCAPE)) break;

        ///////////////////
        /// UPDATE HERE ///
        Scene_Update(mainCharScene, deltatime);
        ///////////////////

        Graphics_DrawTextureFullscreen(&screenCaptureTexture);

        /////////////////
        /// DRAW HERE ///
        Scene_Draw(mainCharScene);
        /////////////////

        Graphics_Flush();

        deltatime = Timer_Reset(&globalTimer);
    }

    Scene_Destroy(mainCharScene);

    free(screenCaptureImage.data);
    Graphics_UnloadTexture(screenCaptureTexture);
    Window_Destroy();

    return 0;
}