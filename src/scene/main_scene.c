#include "main_scene.h"
#include "block_scene.h"
#include "collider_scene.h"
#include "main_char_scene.h"
#include "physics/transform.h"
#include "rect.h"
#include "scene.h"
#include "graphics/graphics.h"
#include "scene_type.h"
#include "vector2.h"
#include <stdlib.h>

static void DrawTestRect()
{
    Vector2 mousePos = Graphics_GetMousePosition();
    Vector2 size = (Vector2){ 50.0f, 50.0f };
    Transform transform = Transform_Zero();
    transform.position = mousePos;
    transform.origin = Vector2_New(25.0f, 25.0f);
    transform.scale = (Vector2){ .x = 2.0f, .y = 1.0f };
    transform.rotation = 45.0f;
    Transform_UpdateMatrix(&transform);

    Graphics_SetModelMatrix(&transform);
    Graphics_DrawRectT(size, COLOR_RED, 1000);
    Graphics_ClearModelMatrix();
}

static void Draw(Scene* scene)
{
    //DrawTestRect();
}

Scene* MainScene_Create()
{
    Scene* scene = malloc(sizeof(Scene));
    
    Scene_DefaultInit(scene, SCENE_TYPE_MAIN, "Main Scene");

    Vector2 screenSize = Graphics_GetScreenSize();

    Scene* upperWorldColliderScene = ColliderScene_Create(scene, screenSize, "Upper Screen Bound Collider");
    upperWorldColliderScene->transform.position.y = screenSize.y;
    Scene* lowerWorldColliderScene = ColliderScene_Create(scene, screenSize, "Lower Screen Bound Collider");
    lowerWorldColliderScene->transform.position.y = -screenSize.y;
    Scene* leftWorldColliderScene = ColliderScene_Create(scene, screenSize, "Left Screen Bound Collider");
    leftWorldColliderScene->transform.position.x = -screenSize.x;
    Scene* rightWorldColliderScene = ColliderScene_Create(scene, screenSize, "Right Screen Bound Collider");
    rightWorldColliderScene->transform.position.x = screenSize.x;

    Scene* mainCharScene = MainCharScene_Create(scene);
    mainCharScene->transform.position = Vector2_MultScalar(Graphics_GetScreenSize(), 0.5f);

    Rect blockSceneRect = {
        .x = 300.0f,
        .y = 800.0f,
        .width = 100.0f,
        .height = 20.0f
    };
    BlockScene_Create(scene, blockSceneRect, COLOR_BLUE);

    scene->drawFunction = Draw;

    return scene;
}