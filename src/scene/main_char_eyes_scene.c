#include "graphics/draw_order.h"
#include "graphics/graphics.h"
#include "graphics/texture_atlas.h"
#include "random.h"
#include "scene.h"
#include "scene_type.h"
#include "tween.h"
#include "vector2.h"
#include "main_char_eyes_scene.h"
#include <stdlib.h>

#define SCENE_TYPE SCENE_TYPE_MAIN_CHAR_EYE

constexpr double c_blinkAnimationLength = 0.2f;
constexpr float c_eyeOffsetY = -25.0f;

typedef struct MainCharEyeSceneData {
    TextureAtlas eyeTextureAtlas;
    double elapsedSinceLastBlink;
    u8 currentTextureAtlasIdx;
    bool inAnimation;
} MainCharEyeSceneData;

static void BlinkAnimationTweenFunction(Scene* scene, double weight, double _)
{
    MainCharEyeSceneData* sceneData = scene->sceneData;

    sceneData->currentTextureAtlasIdx = (u8)(weight * ((float)sceneData->eyeTextureAtlas.texturesCount - 1.0f));
}

static void OnBlinkAnimationFinished(Scene* scene)
{
    MainCharEyeSceneData* sceneData = scene->sceneData;
    sceneData->currentTextureAtlasIdx = 0u;
    sceneData->inAnimation = false;
}

static void Blink(Scene* scene, double deltatime)
{
    constexpr float blinkProbability = 0.05f;

    MainCharEyeSceneData* sceneData = scene->sceneData;

    sceneData->elapsedSinceLastBlink += deltatime;

    if (blinkProbability * deltatime * sceneData->elapsedSinceLastBlink > RandomFloat())
    {
        sceneData->elapsedSinceLastBlink = 0.0f;

        if (sceneData->inAnimation) return;

        TweenHandle tween = Tween_CreateFunction(c_blinkAnimationLength, scene, BlinkAnimationTweenFunction, TWEEN_INTERPOLATION_LINEAR);
        Tween_SetOnFinishCallback(tween, scene, OnBlinkAnimationFinished);
    }
}

static void Update(Scene* scene, double deltatime)
{
    Blink(scene, deltatime);
}

static void Draw(Scene* scene)
{
    MainCharEyeSceneData* sceneData = scene->sceneData;

    Graphics_SetModelMatrix(&scene->globalTransform);
    Graphics_DrawTextureT(TextureAtlas_TextureByIdx(&sceneData->eyeTextureAtlas, sceneData->currentTextureAtlasIdx), DRAW_ORDER_MAIN_CHAR, COLOR_WHITE);
    Graphics_ClearModelMatrix();
}

static void Cleanup(Scene* scene)
{
    MainCharEyeSceneData* sceneData = scene->sceneData;
    TextureAtlas_Free(&sceneData->eyeTextureAtlas);
}

Scene* MainCharEyesScene_Create(Scene* parent, const char* name)
{
    Scene* scene = malloc(sizeof(Scene));
    Scene_DefaultInit(scene, SCENE_TYPE_MAIN_CHAR_EYE, parent, name);

    MainCharEyeSceneData* sceneData = malloc(sizeof(MainCharEyeSceneData));
    sceneData->eyeTextureAtlas = TextureAtlas_New("res/images/main_char/MainCharEyes.png", 8, 1);
    sceneData->elapsedSinceLastBlink = 0.0;
    sceneData->currentTextureAtlasIdx = 0u;
    sceneData->inAnimation = false;

    scene->sceneData = sceneData;
    scene->transform.position.y = c_eyeOffsetY;
    scene->transform.origin = Vector2_MultScalar(Vector2u_ToVector2(sceneData->eyeTextureAtlas.textureSize), 0.5f);

    scene->updateFunction = Update;
    scene->drawFunction = Draw;
    scene->cleanupFunction = Cleanup;

    return scene;
}