#include "edit_mode.h"
#include "cleanup.h"
#include "game_manager.h"
#include "graphics/color.h"
#include "graphics/draw_order.h"
#include "input/input.h"
#include "input/input_button.h"
#include "graphics/graphics.h"
#include "scene/button_scene.h"
#include "scene/label_scene.h"
#include "scene/scene.h"
#include "scene/stack_container_scene.h"
#include "scene/tumor_scene.h"
#include "vector2.h"

constexpr Color c_spawnButtonColor = (Color) { .r = 30.0f, .g = 30.0f, .b = 30.0f, .a = 255.0f };

static bool s_initialized;
static bool s_enabled;
static char* s_cursorPosString;
static const char* s_spawnTooltipString;
static Scene* s_stackContainer;
static Scene* s_spawnTumorButton;
static Scene* s_sceneToSpawn;

static void Cleanup()
{
    free(s_cursorPosString);
}

static void OnTumorSpawnButtonPressed(Scene* scene)
{
    s_sceneToSpawn = TumorScene_Create(NULL);
    s_spawnTooltipString = "Tumor";
}

static void EnsureInitialized()
{
    if (s_initialized) return;

    s_cursorPosString = malloc(128u);
    
    s_sceneToSpawn = NULL;

    s_stackContainer = StackContainerScene_Create(GameManager_GetRootScene(), false, 10.0f, "Edit Mode Stack Container");
    s_stackContainer->transform.position.x = 10.0f;
    s_stackContainer->transform.position.y = 130.0f;
    s_stackContainer->visible = false;

    s_spawnTumorButton = ButtonScene_Create(s_stackContainer, "Spawn Tumor Button", (Vector2) { 100.0f, 30.0f }, c_spawnButtonColor, 
        "Tumor", OnTumorSpawnButtonPressed, GameManager_GetRootScene());
    s_spawnTumorButton->transform.position = (Vector2) { 10.0f, 130.0f };
    ButtonScene_SetBorder(s_spawnTumorButton, 3.0f, COLOR_WHITE);

    Cleanup_AddCallback(Cleanup);

    s_initialized = true;
}

void EditMode_Update(double deltatime)
{
    EnsureInitialized();

    if (Input_IsKeyJustReleased(INPUT_KEY_GRAVE))
    {
        s_enabled = !s_enabled;
        s_stackContainer->visible = !s_stackContainer->visible;
    } 

    if (s_enabled)
    {
        if (s_sceneToSpawn)
        {
            if (Input_IsMouseButtonJustPressed(INPUT_MOUSE_BUTTON_LEFT))
            {
                s_sceneToSpawn->transform.position = Input_GetMousePosition();
                Scene_AddChild(GameManager_GetRootScene(), s_sceneToSpawn);
                s_sceneToSpawn = NULL;
            }
            else if (Input_IsMouseButtonJustPressed(INPUT_MOUSE_BUTTON_RIGHT))
            {
                s_sceneToSpawn = NULL;
            }
        }
    }
}

void EditMode_Draw()
{
    if (!s_enabled) return;

    Graphics_DrawText("Edit Mode", Vector2_New(10.0f, 50.0f), 30.0f, COLOR_WHITE, DRAW_ORDER_UI);
    
    Vector2 mousePos = Input_GetMousePosition();
    sprintf(s_cursorPosString, "Cursor Pos: %d:%d", (u32)mousePos.x, (u32)mousePos.y);
    Graphics_DrawText(s_cursorPosString, Vector2_New(10.0f, 90.0f), 20.0f, COLOR_WHITE, DRAW_ORDER_UI);

    if (s_sceneToSpawn)
        Graphics_DrawText(s_spawnTooltipString, mousePos, 14.0f, COLOR_WHITE, DRAW_ORDER_UI);
}

bool EditMode_Enabled()
{
    return s_enabled;
}