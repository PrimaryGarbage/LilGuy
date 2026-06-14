#include "edit_mode.h"
#include "cleanup.h"
#include "debug.h"
#include "game_manager.h"
#include "graphics/color.h"
#include "graphics/draw_order.h"
#include "input/input.h"
#include "input/input_button.h"
#include "graphics/graphics.h"
#include "scene/block_scene.h"
#include "scene/ui/button_scene.h"
#include "scene/scene.h"
#include "scene/ui/stack_container_scene.h"
#include "scene/tumor_scene.h"
#include "vector2.h"

#define BUTTON_TAG_SPAWN_TUMOR 1
#define BUTTON_TAG_SPAWN_BLOCK 2
#define BUTTON_TAG_SPAWN_PRINT_SCENE_TREE 3

constexpr Color c_spawnButtonColor = (Color) { .r = 30.0f, .g = 30.0f, .b = 30.0f, .a = 255.0f };
constexpr Color c_printButtonColor = (Color) { .r = 30.0f, .g = 180.0f, .b = 30.0f, .a = 255.0f };

static bool s_initialized;
static bool s_enabled;
static char* s_cursorPosString;
static const char* s_spawnTooltipString;
static Scene* s_stackContainer;
static Scene* s_spawnTumorButton;
static Scene* s_spawnBlockButton;
static Scene* s_sceneToSpawn;
static Scene* s_printSceneTreeButton;

static void Cleanup()
{
    free(s_cursorPosString);
}

static void OnButtonPressed(Scene* scene, i32 tag)
{
    switch(tag)
    {
        case BUTTON_TAG_SPAWN_TUMOR:
            s_sceneToSpawn = TumorScene_Create(NULL);
            s_spawnTooltipString = "Tumor";
            break;
        case BUTTON_TAG_SPAWN_BLOCK:
            s_sceneToSpawn = BlockScene_Create(NULL);
            s_spawnTooltipString = "Block";
            break;
        case BUTTON_TAG_SPAWN_PRINT_SCENE_TREE:
            Debug_PrintSceneTree(GameManager_GetRootScene());
            break;
    }
}

static void EnsureInitialized()
{
    if (s_initialized) return;

    s_cursorPosString = malloc(128u);

    Scene* rootScene = GameManager_GetRootScene();
    
    s_sceneToSpawn = NULL;

    s_stackContainer = StackContainerScene_Create(GameManager_GetRootScene(), false, 10.0f, "Edit Mode Stack Container");
    s_stackContainer->transform.position.x = 10.0f;
    s_stackContainer->transform.position.y = 130.0f;
    s_stackContainer->enabled = false;
    StackContainerScene_SetBackground(s_stackContainer, (Color){ .r = 0u, .g = 0u, .b = 0u, .a = 100u });

    s_spawnTumorButton = ButtonScene_Create(s_stackContainer, "Spawn Tumor Button", (Vector2){ 100.0f, 30.0f }, c_spawnButtonColor,
        "Tumor", BUTTON_TAG_SPAWN_TUMOR, OnButtonPressed, s_stackContainer);
    ButtonScene_SetBorder(s_spawnTumorButton, 3.0f, COLOR_WHITE);

    s_spawnBlockButton = ButtonScene_Create(s_stackContainer, "Spawn Block Button", (Vector2) { 100.0f, 30.0f }, c_spawnButtonColor, 
        "Block", BUTTON_TAG_SPAWN_BLOCK, OnButtonPressed, s_stackContainer);
    ButtonScene_SetBorder(s_spawnBlockButton, 3.0f, COLOR_WHITE);

    s_printSceneTreeButton = ButtonScene_Create(s_stackContainer, "Print Scene Tree Button", (Vector2) { 100.0f, 30.0f }, c_printButtonColor,
        "Print Scene Tree", BUTTON_TAG_SPAWN_PRINT_SCENE_TREE, OnButtonPressed, s_stackContainer);

    Cleanup_AddCallback(Cleanup);

    s_initialized = true;
}

void EditMode_Update(double deltatime)
{
    EnsureInitialized();

    if (Input_IsKeyJustReleased(INPUT_KEY_GRAVE))
    {
        s_enabled = !s_enabled;
        s_stackContainer->enabled = s_enabled;
        GameManager_PauseGame(s_enabled);
    } 

    if (s_enabled)
    {
        if (s_sceneToSpawn)
        {
            s_sceneToSpawn->transform.rotation += Input_GetMouseWheelMove();

            if (Input_IsMouseButtonJustPressed(INPUT_MOUSE_BUTTON_LEFT))
            {
                s_sceneToSpawn->transform.position = Input_GetMousePosition();
                Scene_AddChild(GameManager_GetGenericSpawnParentScene(), s_sceneToSpawn);
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
        Graphics_DrawTextRotated(s_spawnTooltipString, mousePos, 14.0f, s_sceneToSpawn->transform.rotation, COLOR_WHITE, DRAW_ORDER_UI);
}

bool EditMode_Enabled()
{
    return s_enabled;
}