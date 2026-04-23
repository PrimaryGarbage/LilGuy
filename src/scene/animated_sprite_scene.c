#include "animated_sprite_scene.h"
#include "graphics/graphics.h"
#include "graphics/texture_atlas.h"
#include "scene.h"
#include "scene_type.h"
#include <stdlib.h>

#define SCENE_TYPE SCENE_TYPE_ANIMATED_SPRITE

typedef struct AnimatedSpriteSceneData{
    TextureAtlas textureAtlas;
    u32 currentTextureIdx;
    double elapsed;
    double switchTime;
    u32 drawOrder;
    Color tint;
} AnimatedSpriteSceneData;

static void Update(Scene* scene, double deltatime)
{
    AnimatedSpriteSceneData* sceneData = scene->sceneData;

    sceneData->elapsed += deltatime;
    if (sceneData->elapsed > sceneData->switchTime)
    {
        if (sceneData->currentTextureIdx == sceneData->textureAtlas.texturesCount - 1)
            sceneData->currentTextureIdx = 0u;
        else
            sceneData->currentTextureIdx++;

        sceneData->elapsed = 0.0;
    }
}

static void Draw(Scene* scene)
{
    AnimatedSpriteSceneData* sceneData = scene->sceneData;

    Graphics_SetModelMatrix(&scene->globalTransform);
    Graphics_DrawTextureT(TextureAtlas_GetTextureByIdx(&sceneData->textureAtlas, sceneData->currentTextureIdx), sceneData->drawOrder, sceneData->tint);
    Graphics_ClearModelMatrix();
}

static void Cleanup(Scene* scene)
{
    AnimatedSpriteSceneData* sceneData = scene->sceneData;
    
    TextureAtlas_Free(&sceneData->textureAtlas);
}

Scene* AnimatedSpriteScene_Create(Scene* parent, const char* imagePath, u32 atlasWidth, u32 atlasHeight, double switchTime, u32 drawOrder, const char* sceneName)
{
    Scene* scene = malloc(sizeof(Scene));
    Scene_DefaultInit(scene, SCENE_TYPE_ANIMATED_SPRITE, parent, sceneName);
    AnimatedSpriteSceneData* sceneData = malloc(sizeof(AnimatedSpriteSceneData));
    scene->sceneData = sceneData;

    sceneData->textureAtlas = TextureAtlas_New(imagePath, atlasWidth, atlasHeight);
    sceneData->switchTime = switchTime;
    sceneData->elapsed = 0.0;
    sceneData->currentTextureIdx = 0u;
    sceneData->drawOrder = drawOrder;

    scene->updateFunction = Update;
    scene->drawFunction = Draw;
    scene->cleanupFunction = Cleanup;

    return scene;
}

Vector2 AnimatedSpriteScene_GetSpriteSize(const Scene* scene)
{
    ASSERT_SCENE_TYPE(scene);

    AnimatedSpriteSceneData* sceneData = scene->sceneData;
    return Vector2u_ToVector2(sceneData->textureAtlas.textureSize);
}

void AnimatedSpriteScene_SetTint(Scene* scene, Color tint)
{
    ASSERT_SCENE_TYPE(scene);

    ((AnimatedSpriteSceneData*)scene->sceneData)->tint = tint;
}
