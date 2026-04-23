#include "block_scene.h"
#include "collider_scene.h"
#include "graphics/draw_order.h"
#include "graphics/graphics.h"
#include "rect.h"
#include "scene.h"
#include <stdlib.h>

#define SCENE_TYPE SCENE_TYPE_BLOCK

typedef struct BlockSceneData {
    Color color;
    Vector2 size;
} BlockSceneData;

static void Draw(Scene* scene)
{
    BlockSceneData* sceneData = scene->sceneData;

    Graphics_SetModelMatrix(&scene->globalTransform);
    Graphics_DrawRectT(sceneData->size, sceneData->color, DRAW_ORDER_DEFAULT);
    Graphics_ClearModelMatrix();
}

Scene* BlockScene_Create(Scene* parent, Rect rect, Color color)
{
    Scene* scene = malloc(sizeof(Scene));
    Scene_DefaultInit(scene, SCENE_TYPE_BLOCK, parent, "Block");

    BlockSceneData* sceneData = malloc(sizeof(BlockSceneData));
    sceneData->color = color;
    sceneData->size = Rect_GetSize(&rect);

    scene->sceneData = sceneData;
    scene->transform.position = Rect_GetPosition(&rect);

    Scene* colliderScene = ColliderScene_Create(scene, Rect_GetSize(&rect), "Block Scene Collider");
    ColliderScene_SetCollisionLayers(colliderScene, COLLIDER_LAYER_WORLD);
    ColliderScene_SetCollisionScan(colliderScene, COLLIDER_LAYER_WORLD);

    Scene_UpdateGlobalTransform(scene);

    scene->drawFunction = Draw;

    return scene;
}