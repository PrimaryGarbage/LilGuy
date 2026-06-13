#include "cleanup.h"
#include "debug.h"
#include "edit_mode.h"
#include "game_manager.h"
#include "graphics/draw_order.h"
#include "graphics/image.h"
#include "scene/main_scene.h"
#include "scene/scene.h"
#include "tween.h"
#include "vector2.h"
#include "window/mouse_cursor_type.h"
#include "window/window.h"
#include "graphics/graphics.h"
#include "random.h"
#include "result.h"
#include "timer.h"
#include "logging.h"

static void Update(double deltatime)
{
    EditMode_Update(deltatime);

    double simulationDeltatime = EditMode_Enabled() ? 0.0 : deltatime;
    Scene_Update(GameManager_GetRootScene(), simulationDeltatime);
    Tween_Update(simulationDeltatime);
}

static void Draw()
{
    EditMode_Draw();
    Scene_Draw(GameManager_GetRootScene());
}

int main()
{
    RandomInit();
    Timer globalTimer = Timer_Create();
    double deltatime;

    Image screenCaptureImage;
    Result captureResult = Graphics_CaptureScreen(&screenCaptureImage);
    IF_ERROR_PANIC_EX(captureResult, LogError(&captureResult, NULL););

    Vector2u windowSize = { screenCaptureImage.width, screenCaptureImage.height };

    Window_Init("LilGuy", windowSize, true);
    //Window_Hide();
    Window_SetMonitor(1);
    Window_SetMouseCursor(MOUSE_CURSOR_TYPE_CROSSHAIR);

    Texture2D screenCaptureTexture = Graphics_LoadTextureFromImage(&screenCaptureImage);
    Graphics_DrawTexture(&screenCaptureTexture, (Rect){ .x = 0.0f, .y = 0.0f, .width = screenCaptureTexture.width, .height = screenCaptureTexture.height }, DRAW_ORDER_BACKGROUND);
    Graphics_Flush();
    Window_Show();

    Scene* rootScene = MainScene_Create();

    GameManager_Init(rootScene);
    Scene_Start(rootScene);

    Debug_PrintSceneTree(rootScene);

    while(!Window_ShouldClose())
    {
        Window_PollEvents();

        Graphics_ClearBackground(COLOR_BLUE);
        Graphics_DrawTexture(&screenCaptureTexture, (Rect){ .x = 0.0f, .y = 0.0f, .width = screenCaptureTexture.width, .height = screenCaptureTexture.height }, DRAW_ORDER_BACKGROUND);

        ///////////////////
        /// UPDATE HERE ///
        Update(deltatime);
        ///////////////////

        /////////////////
        /// DRAW HERE ///
        Draw();
        /////////////////

        Graphics_Flush();

        Scene_TrimQueuedScenes();

        deltatime = Timer_Reset(&globalTimer);
    }

    Scene_Free(rootScene);

    Cleanup_Execute();

    Image_Free(&screenCaptureImage);
    Graphics_UnloadTexture(screenCaptureTexture);
    Window_Destroy();

    return 0;
}