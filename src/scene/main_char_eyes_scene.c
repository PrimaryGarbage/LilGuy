#include "graphics/draw_order.h"
#include "graphics/graphics.h"
#include "graphics/texture_atlas.h"
#include "math_helpers.h"
#include "random.h"
#include "scene.h"
#include "scene_type.h"
#include "tween.h"
#include "vector2.h"
#include "main_char_eyes_scene.h"
#include <stdlib.h>

#define SCENE_TYPE SCENE_TYPE_MAIN_CHAR_EYE

constexpr double c_blinkAnimationLength = 0.2;
constexpr double c_squintMaxTime = 1.0;
constexpr float c_eyeOffsetY = -25.0f;

typedef struct MainCharEyeSceneData {
    TextureAtlas eyeTextureAtlas;
    Texture2D eyesSquintingTexture;
    double elapsedSinceLastBlink;
    u8 currentTextureAtlasIdx;
    bool inAnimation;
    double squintingTime;
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
    sceneData->squintingTime -= deltatime;
    sceneData->squintingTime = Clampd(0.0, c_squintMaxTime, sceneData->squintingTime);

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

    Texture2D* texture = sceneData->squintingTime > 0.0 ? &sceneData->eyesSquintingTexture : TextureAtlas_TextureByIdx(&sceneData->eyeTextureAtlas, sceneData->currentTextureAtlasIdx);

    Graphics_SetModelMatrix(&scene->globalTransform);
    Graphics_DrawTextureT(texture, DRAW_ORDER_MAIN_CHAR, COLOR_WHITE);
    Graphics_ClearModelMatrix();
}

static void Cleanup(Scene* scene)
{
    MainCharEyeSceneData* sceneData = scene->sceneData;
    TextureAtlas_Free(&sceneData->eyeTextureAtlas);
    Graphics_UnloadTexture(sceneData->eyesSquintingTexture);
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
    sceneData->squintingTime = 0.0;
    sceneData->eyesSquintingTexture = Graphics_LoadTexture("res/images/main_char/MainCharEyeShooting.png");

    scene->sceneData = sceneData;
    scene->transform.position.y = c_eyeOffsetY;
    scene->transform.origin = Vector2_MultScalar(Vector2u_ToVector2(sceneData->eyeTextureAtlas.textureSize), 0.5f);

    scene->updateFunction = Update;
    scene->drawFunction = Draw;
    scene->cleanupFunction = Cleanup;

    return scene;
}

void MainCharEyesScene_Squint(Scene* scene)
{
    ASSERT_SCENE_TYPE(scene);

    MainCharEyeSceneData* sceneData = scene->sceneData;

    sceneData->squintingTime = c_squintMaxTime;
}