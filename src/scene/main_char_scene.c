#include "main_char_scene.h"
#include "input/input.h"
#include "graphics/graphics.h"
#include "rect.h"
#include "vector2.h"
#include <stdlib.h>

#define CHAR_SIZE 50

typedef struct MainChairSceneCustomData {

} MainCharSceneCustomData;

static void UpdateCallback(Scene* scene, double deltatime)
{
    const float moveSpeed = 500.0f;

    if (Input_IsKeyPressed(INPUT_KEY_A)) {
        scene->transform.position.x -= moveSpeed * deltatime;
    }
    if (Input_IsKeyPressed(INPUT_KEY_D)) {
        scene->transform.position.x += moveSpeed * deltatime;
    }
    if (Input_IsKeyPressed(INPUT_KEY_W)) {
        scene->transform.position.y -= moveSpeed * deltatime;
    }
    if (Input_IsKeyPressed(INPUT_KEY_S)) {
        scene->transform.position.y += moveSpeed * deltatime;
    }
}

static void DrawCallback(Scene* scene)
{
    Graphics_SetTransform(&scene->transform);
    Graphics_DrawRect(Rect_FromVectors(scene->transform.position, Vector2_Uniform(CHAR_SIZE)), COLOR_PURPLE);
    Graphics_ClearTransform();
}

Scene* MainCharScene_Create()
{
    Scene* scene = malloc(sizeof(Scene));

    scene->updateFunction = UpdateCallback;
    scene->drawFunction = DrawCallback;
    scene->customData = malloc(sizeof(MainCharSceneCustomData));
    scene->transform.position = Vector2_Uniform(100.0f);
    scene->transform.scale = Vector2_One();
    scene->transform.origin = Vector2_Uniform(CHAR_SIZE / 2.0f);
    scene->childrenCount = 0u;

    return scene;
}