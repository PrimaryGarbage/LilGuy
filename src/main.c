#include "graphics/image.h"
#include "input/input.h"
#include "input/input_button.h"
#include "scene/empty_scene.h"
#include "scene/scene.h"
#include "twine.h"
#include "vector2.h"
#include "window/window.h"
#include "graphics/graphics.h"
#include "random.h"
#include "result.h"
#include "timer.h"
#include "logging.h"
#include "scene/main_char_scene.h"

int main(int argc, char* argv[])
{
    RandomInit();
    Timer globalTimer = Timer_Create();
    double deltatime;

    Image screenCaptureImage;
    Result captureResult = Graphics_CaptureScreen(&screenCaptureImage);
    IF_ERROR_PANIC_EX(captureResult, LogError(&captureResult, NULL););

    Vector2u windowSize = { screenCaptureImage.width, screenCaptureImage.height };

    Window_Hide();
    Window_Init("LilGuy", windowSize, true);
    Window_SetMonitor(1);

    Texture2D screenCaptureTexture = Graphics_LoadTextureFromImage(&screenCaptureImage);
    Graphics_DrawTextureFullscreen(&screenCaptureTexture);
    Graphics_Flush();
    Window_Show();

    Scene* rootScene = EmptyScene_Create();
    Scene* mainCharScene = MainCharScene_Create(NULL);
    Scene_AddChild(rootScene, mainCharScene);

    Scene_Start(rootScene);

    while(!Window_ShouldClose())
    {
        Window_PollEvents();

        if (Input_IsKeyPressed(INPUT_KEY_ESCAPE)) break;

        ///////////////////
        /// UPDATE HERE ///
        Scene_Update(rootScene, deltatime);
        ///////////////////

        Twine_Update(deltatime);

        Graphics_ClearBackground(COLOR_BLUE);
        Graphics_DrawTextureFullscreen(&screenCaptureTexture);

        /////////////////
        /// DRAW HERE ///
        Scene_Draw(rootScene);
        /////////////////
        
        Graphics_Flush();

        deltatime = Timer_Reset(&globalTimer);
    }

    Scene_Free(mainCharScene);

    Image_Free(&screenCaptureImage);
    Graphics_UnloadTexture(screenCaptureTexture);
    Window_Destroy();

    return 0;
}