#include "debug.h"
#include "graphics/draw_order.h"
#include "graphics/image.h"
#include "input/input.h"
#include "input/input_button.h"
#include "scene/main_scene.h"
#include "tween.h"
#include "vector2.h"
#include "window/window.h"
#include "graphics/graphics.h"
#include "random.h"
#include "result.h"
#include "timer.h"
#include "logging.h"

int main()
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
    Graphics_DrawTexture(&screenCaptureTexture, (Rect){ .x = 0.0f, .y = 0.0f, .width = screenCaptureTexture.width, .height = screenCaptureTexture.height }, DRAW_ORDER_BACKGROUND);
    Graphics_Flush();
    Window_Show();

    Scene* rootScene = MainScene_Create();

    Scene_Start(rootScene);

    Debug_PrintSceneTree(rootScene);

    while(!Window_ShouldClose())
    {
        Window_PollEvents();

        if (Input_IsKeyPressed(INPUT_KEY_ESCAPE)) break;

        Graphics_ClearBackground(COLOR_BLUE);
        Graphics_DrawTexture(&screenCaptureTexture, (Rect){ .x = 0.0f, .y = 0.0f, .width = screenCaptureTexture.width, .height = screenCaptureTexture.height }, DRAW_ORDER_BACKGROUND);

        ///////////////////
        /// UPDATE HERE ///
        Scene_Update(rootScene, deltatime);
        ///////////////////

        Tween_Update(deltatime);

        /////////////////
        /// DRAW HERE ///
        Scene_Draw(rootScene);
        /////////////////
        
        Graphics_Flush();

        deltatime = Timer_Reset(&globalTimer);
    }

    Scene_Free(rootScene);

    Image_Free(&screenCaptureImage);
    Graphics_UnloadTexture(screenCaptureTexture);
    Window_Destroy();

    return 0;
}