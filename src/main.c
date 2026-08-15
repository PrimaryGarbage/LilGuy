#include "cleanup.h"
#include "debug.h"
#include "edit_mode.h"
#include "game_manager.h"
#include "graphics/draw_order.h"
#include "graphics/image.h"
#include "scene/root_scene.h"
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

static Texture2D s_screenCaptureTexture;

static void Update()
{
    double deltatime = GameManager_GetDeltatime();

    EditMode_Update(deltatime);
    Scene_Update(GameManager_GetRootScene(), deltatime);
    Tween_Update(deltatime);
}

static void Draw()
{
    Graphics_ClearBackground(COLOR_BLUE);
    Graphics_DrawTexture(&s_screenCaptureTexture, (Rect){ .x = 0.0f, .y = 0.0f, 
        .width = s_screenCaptureTexture.width, .height = s_screenCaptureTexture.height }, DRAW_ORDER_BACKGROUND);

    EditMode_Draw();
    Scene_Draw(GameManager_GetRootScene());

    Graphics_Flush();
}

int main()
{
    RandomInit();

    Image screenCaptureImage;
    Result captureResult = Graphics_CaptureScreen(&screenCaptureImage);
    IF_ERROR_PANIC_EX(captureResult, LogError(&captureResult, NULL););

    Vector2u windowSize = { screenCaptureImage.width, screenCaptureImage.height };

    Window_Init("LilGuy", windowSize, true);
    //Window_Hide();
    //Window_SetMonitor(1);
    Window_SetMouseCursor(MOUSE_CURSOR_TYPE_CROSSHAIR);

    s_screenCaptureTexture = Graphics_LoadTextureFromImage(&screenCaptureImage);
    Image_Free(&screenCaptureImage);
    Graphics_DrawTexture(&s_screenCaptureTexture, (Rect){ .x = 0.0f, .y = 0.0f, .width = s_screenCaptureTexture.width, .height = s_screenCaptureTexture.height }, DRAW_ORDER_BACKGROUND);
    Graphics_Flush();
    Window_Show();

    Scene* rootScene = RootScene_Create();
    GameManager_Init(rootScene);
    Scene_Start(rootScene);

    Debug_PrintSceneTree(rootScene);

    Timer globalTimer = Timer_Create();

    while(!Window_ShouldClose())
    {
        Window_PollEvents();

        Update();

        Draw();

        Scene_TrimQueuedScenes();

        GameManager_SetDeltatime(Timer_Reset(&globalTimer));
    }

    Scene_Free(rootScene);

    Graphics_UnloadTexture(s_screenCaptureTexture);
    Cleanup_Execute();
    Window_Destroy();

    return 0;
}