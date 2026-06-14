#include "exit_menu_scene.h"
#include "game_manager.h"
#include "graphics/color.h"
#include "input/input.h"
#include "input/input_button.h"
#include "scene/scene.h"
#include "scene/ui/button_scene.h"
#include "scene/ui/stack_container_scene.h"
#include "scene/ui/label_scene.h"
#include "vector2.h"
#include <stdlib.h>

typedef struct {
    Scene* rootContainer;
} ExitMenuSceneData;

static void OnPressedYes(Scene* scene, i32 tag)
{
    (void)scene;
    (void)tag;

    GameManager_RequestExit();
}

static void OnPressedNo(Scene* scene, i32 tag)
{
    (void)tag;

    ExitMenuSceneData* sceneData = scene->sceneData;

    sceneData->rootContainer->enabled = false;
    GameManager_PauseGame(false);
}


static void Update(Scene* scene, double deltatime)
{
    (void)deltatime;

    ExitMenuSceneData* sceneData = scene->sceneData;

    if (Input_IsKeyJustPressed(INPUT_KEY_ESCAPE))
    {
        sceneData->rootContainer->enabled = !sceneData->rootContainer->enabled;
        sceneData->rootContainer->transform.position = Input_GetMousePosition();
        
        if (sceneData->rootContainer->enabled)
            GameManager_PauseGame(true);
    }
}

Scene* ExitMenuScene_Create(Scene* parent)
{
    Scene* scene = malloc(sizeof(Scene));
    Scene_DefaultInit(scene, SCENE_TYPE_EXIT_MENU, parent, "Exit Menu");
    ExitMenuSceneData* sceneData = malloc(sizeof(ExitMenuSceneData));
    scene->sceneData = sceneData;

    Scene* rootContainer = StackContainerScene_Create(scene, false, 5.0f, "Exit Menu Parent");
    sceneData->rootContainer = rootContainer;
    LabelScene_Create(rootContainer, "Exit Menu Label", "Exit Game?", 20.0f, COLOR_WHITE);
    Scene* buttonContainer = StackContainerScene_Create(rootContainer, true, 5.0f, "Exit Menu Button Container");
    ButtonScene_Create(buttonContainer, "Exit Menu Button Yes", (Vector2) { 60.0f, 30.0f }, COLOR_BLACK, "Yes", 0, OnPressedYes, scene);
    ButtonScene_Create(buttonContainer, "Exit Menu Button No", (Vector2) { 60.0f, 30.0f }, COLOR_BLACK, "No", 0, OnPressedNo, scene);

    rootContainer->enabled = false;

    scene->updateFunction = Update;

    return scene;
}
