#include "main_scene.h"
#include "block_scene.h"
#include "collider_scene.h"
#include "main_char_scene.h"
#include "scene.h"
#include "graphics/graphics.h"
#include "scene_type.h"
#include <stdlib.h>

Scene* MainScene_Create()
{
    Scene* scene = malloc(sizeof(Scene));
    
    Scene_DefaultInit(scene, SCENE_TYPE_MAIN, "Main Scene");

    Vector2 screenSize = Graphics_GetScreenSize();

    Scene* upperWorldColliderScene = ColliderScene_Create(scene, screenSize, "Upper Screen Bound Collider");
    upperWorldColliderScene->transform.position.y = -screenSize.y;
    Scene* lowerWorldColliderScene = ColliderScene_Create(scene, screenSize, "Lower Screen Bound Collider");
    lowerWorldColliderScene->transform.position.y = screenSize.y;
    Scene* leftWorldColliderScene = ColliderScene_Create(scene, screenSize, "Left Screen Bound Collider");
    leftWorldColliderScene->transform.position.x = -screenSize.x;
    Scene* rightWorldColliderScene = ColliderScene_Create(scene, screenSize, "Right Screen Bound Collider");
    rightWorldColliderScene->transform.position.x = screenSize.x;
    Scene_AddChild(scene, upperWorldColliderScene);
    Scene_AddChild(scene, lowerWorldColliderScene);
    Scene_AddChild(scene, leftWorldColliderScene);
    Scene_AddChild(scene, rightWorldColliderScene);

    Scene* mainCharScene = MainCharScene_Create(scene);
    mainCharScene->transform.position = Vector2_MultScalar(Graphics_GetScreenSize(), 0.5f);
    Scene_AddChild(scene, mainCharScene);

    Rect blockSceneRect = {
        .x = 300.0f,
        .y = 900.0f,
        .width = 100.0f,
        .height = 20.0f
    };
    Scene* blockScene = BlockScene_Create(scene, blockSceneRect, COLOR_BLUE);
    Scene_AddChild(scene, blockScene);

    return scene;
}
