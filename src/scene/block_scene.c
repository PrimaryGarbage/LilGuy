#include "block_scene.h"
#include "collider_scene.h"
#include "graphics/graphics.h"
#include "rect.h"
#include "scene.h"
#include <stdlib.h>

typedef struct BlockSceneData {
    Color color;
    Vector2 size;
} BlockSceneData;

static void Draw(Scene* scene)
{
    BlockSceneData* sceneData = scene->sceneData;

    Graphics_SetTransform(&scene->globalTransform);
    Graphics_DrawRectT(sceneData->size, sceneData->color);
    Graphics_ClearTransform();
}

Scene* BlockScene_Create(Scene* parent, Rect rect, Color color)
{
    Scene* scene = malloc(sizeof(Scene));

    BlockSceneData* sceneData = malloc(sizeof(BlockSceneData));
    sceneData->color = color;
    sceneData->size = Rect_GetSize(&rect);

    scene->sceneData = sceneData;
    scene->type = SCENE_TYPE_BLOCK;
    scene->transform.position = Rect_GetPosition(&rect);
    scene->transform.scale = Vector2_Uniform(1.0f);
    scene->transform.origin = Vector2_Zero();
    scene->transform.rotation = 0.0f;
    scene->transform.topLevel = false;
    scene->childrenCount = 0u;
    scene->name = "Block Scene";
    scene->parent = parent;

    Scene* colliderScene = ColliderScene_Create(scene, Rect_GetSize(&rect), "Block Scene Collider");
    ColliderScene_SetCollisionLayers(colliderScene, COLLIDER_LAYER_WORLD);
    ColliderScene_SetCollisionScan(colliderScene, COLLIDER_LAYER_WORLD);

    Scene_AddChild(scene, colliderScene);

    Scene_UpdateGlobalTransform(scene, false);

    scene->startFunction = NULL;
    scene->updateFunction = NULL;
    scene->drawFunction = Draw;
    scene->cleanupFunction = NULL;

    return scene;
}
