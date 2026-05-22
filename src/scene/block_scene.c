#include "block_scene.h"
#include "collider_scene.h"
#include "graphics/draw_order.h"
#include "graphics/graphics.h"
#include "physics/collision_layer.h"
#include "scene.h"
#include <stdlib.h>

#define SCENE_TYPE SCENE_TYPE_BLOCK

typedef struct BlockSceneData {
    Texture2D texture;
} BlockSceneData;

static void Draw(Scene* scene)
{
    BlockSceneData* sceneData = scene->sceneData;

    Graphics_SetModelMatrix(&scene->globalTransform);
    Graphics_DrawTextureT(&sceneData->texture, DRAW_ORDER_DEFAULT, COLOR_WHITE);
    //Graphics_DrawRectT(sceneData->size, sceneData->color, DRAW_ORDER_DEFAULT);
    Graphics_ClearModelMatrix();
}

static void Cleanup(Scene* scene)
{
    BlockSceneData* sceneData = scene->sceneData;

    Graphics_UnloadTexture(sceneData->texture);
}

Scene* BlockScene_Create(Scene* parent)
{
    Scene* scene = malloc(sizeof(Scene));
    Scene_DefaultInit(scene, SCENE_TYPE_BLOCK, parent, "Block");

    BlockSceneData* sceneData = malloc(sizeof(BlockSceneData));
    scene->sceneData = sceneData;
    sceneData->texture = Graphics_LoadTexture("res/images/Block.png");

    Scene* colliderScene = ColliderScene_Create(scene, scene, (Vector2) { sceneData->texture.width, sceneData->texture.height }, "Block Scene Collider");
    ColliderScene_SetCollisionLayers(colliderScene, COLLISION_LAYER_WORLD);
    ColliderScene_SetCollisionScan(colliderScene, COLLISION_LAYER_WORLD);

    Scene_UpdateGlobalTransform(scene);

    scene->drawFunction = Draw;
    scene->cleanupFunction = Cleanup;

    return scene;
}