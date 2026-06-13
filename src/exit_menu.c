#include "exit_menu.h"
#include "game_manager.h"
#include "graphics/color.h"
#include "graphics/graphics.h"
#include "input/input.h"
#include "input/input_button.h"
#include "scene/scene.h"
#include "scene/ui/button_scene.h"
#include "scene/ui/stack_container_scene.h"
#include "scene/ui/label_scene.h"
#include "vector2.h"

static Scene* s_menuScene = NULL;
static bool s_shouldExit = false;
static bool s_initialized = false;

static void OnPressedYes(Scene* scene, i32 tag)
{
    s_shouldExit = true;
}

static void OnPressedNo(Scene* scene, i32 tag)
{
    s_menuScene->enabled = false;
}

static void EnsureInitialized()
{
    if (s_initialized) return;

    s_menuScene = StackContainerScene_Create(NULL, false, 5.0f, "Exit Menu Parent");
    LabelScene_Create(s_menuScene, "Exit Menu Label", "Do you wish to exit?", 20.0f, COLOR_WHITE);
    Scene* buttonContainer = StackContainerScene_Create(s_menuScene, true, 5.0f, "Exit Menu Button Container");
    ButtonScene_Create(buttonContainer, "Exit Menu Button Yes", (Vector2) { 60.0f, 30.0f }, COLOR_BLACK, "Yes", 0, OnPressedYes, s_menuScene);
    ButtonScene_Create(buttonContainer, "Exit Menu Button No", (Vector2) { 60.0f, 30.0f }, COLOR_BLACK, "No", 0, OnPressedNo, s_menuScene);

    Scene_AddChild(GameManager_GetRootScene(), s_menuScene);

    s_menuScene->enabled = false;

    s_initialized = true;
}

bool ExitMenu_Update()
{
    EnsureInitialized();

    if (Input_IsKeyJustPressed(INPUT_KEY_ESCAPE))
    {
        s_menuScene->enabled = !s_menuScene->enabled;
        s_menuScene->transform.position = Input_GetMousePosition();
    }

    return s_shouldExit;
}
