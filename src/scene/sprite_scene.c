#include "sprite_scene.h"
#include "graphics/draw_order.h"
#include "graphics/graphics.h"
#include "graphics/texture2d.h"
#include "logging.h"
#include "scene/scene.h"
#include "scene_type.h"
#include <stdlib.h>

typedef struct SpriteSceneData {
    Texture2D texture;
    i32 drawOrder;
    bool shouldUnloadTexture;
} SpriteSceneData;

static void Draw(Scene* scene)
{
    SpriteSceneData* sceneData = scene->sceneData;

    Graphics_SetModelMatrix(&scene->globalTransform);
    Graphics_DrawTextureT(&sceneData->texture, sceneData->drawOrder, COLOR_WHITE);
    Graphics_ClearModelMatrix();
}

static void Cleanup(Scene* scene)
{
    SpriteSceneData* sceneData = scene->sceneData;

    if (sceneData->shouldUnloadTexture)
        Graphics_UnloadTexture(sceneData->texture);
}

Scene* SpriteScene_Create(Scene* parent, const char* imagePath, const char* name)
{
    Scene* scene = malloc(sizeof(Scene));
    Scene_DefaultInit(scene, SCENE_TYPE_SPRITE, parent, name);

    SpriteSceneData* sceneData = malloc(sizeof(SpriteSceneData));
    scene->sceneData = sceneData;
    sceneData->drawOrder = DRAW_ORDER_DEFAULT;
    sceneData->shouldUnloadTexture = true;
    sceneData->texture = Graphics_LoadTexture(imagePath);
    if (sceneData->texture.id == 0)
        PANIC_EX(LogErrorM("Failed to load texture. Filepath: %s", imagePath););

    scene->drawFunction = Draw;
    scene->cleanupFunction = Cleanup;

    return scene;
}

Scene* SpriteScene_CreateWithTexture(Scene* parent, const Texture2D* texture, const char* name)
{
    Scene* scene = malloc(sizeof(Scene));
    Scene_DefaultInit(scene, SCENE_TYPE_SPRITE, parent, name);

    SpriteSceneData* sceneData = malloc(sizeof(SpriteSceneData));
    scene->sceneData = sceneData;
    sceneData->drawOrder = DRAW_ORDER_DEFAULT;
    sceneData->shouldUnloadTexture = false;
    sceneData->texture = *texture;

    scene->drawFunction = Draw;
    scene->cleanupFunction = Cleanup;

    return scene;
}

void SpriteScene_SetDrawOrder(Scene* scene, i32 drawOrder)
{
    ASSERT_SCENE_TYPE(scene, SCENE_TYPE_SPRITE);

    ((SpriteSceneData*)scene->sceneData)->drawOrder = drawOrder;
}
