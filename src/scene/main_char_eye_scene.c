#include "graphics/draw_order.h"
#include "graphics/graphics.h"
#include "scene.h"
#include "scene_type.h"
#include "tween.h"
#include "vector2.h"
#include "main_char_eye_scene.h"
#include <stdlib.h>

constexpr double c_blinkAnimationLength = 0.2f;

typedef struct MainCharEyeSceneData {
    Texture2D eyeTexture;
    float eyelidHeight;
    TweenHandle blinkTween;
} MainCharEyeSceneData;

static void BlinkAnimationTweenFunction(Scene* scene, double weight, double elapsed)
{
    MainCharEyeSceneData* sceneData = scene->sceneData;

    float offsetWeight = weight < 0.5f ? weight * 2.0f : 2.0f - weight * 2.0f;

    sceneData->eyelidHeight = Clampf(0.0f, sceneData->eyeTexture.height, sceneData->eyeTexture.height  * offsetWeight);
}

static void OnBlinkAnimationFinished(Scene* scene)
{
    MainCharEyeSceneData* sceneData = scene->sceneData;
    sceneData->eyelidHeight = 0.0f;
    sceneData->blinkTween = NULL;
}

static void Draw(Scene* scene)
{
    MainCharEyeSceneData* sceneData = scene->sceneData;

    Graphics_SetModelMatrix(&scene->globalTransform);
    Graphics_DrawTextureT(&sceneData->eyeTexture, DRAW_ORDER_MAIN_CHAR, COLOR_WHITE);
    Graphics_ClearModelMatrix();

    // eyelids
    Graphics_DrawRect((Rect){
        .x = scene->globalTransform.position.x - scene->globalTransform.origin.x, 
        .y = scene->globalTransform.position.y - scene->globalTransform.origin.y,
        .width = sceneData->eyeTexture.width,
        .height = sceneData->eyelidHeight
    }, COLOR_BLACK, DRAW_ORDER_MAIN_CHAR);
}

static void Cleanup(Scene* scene)
{
    MainCharEyeSceneData* sceneData = scene->sceneData;
    Graphics_UnloadTexture(sceneData->eyeTexture);
}

Scene* MainCharEyeScene_Create(Scene* parent, const char* name)
{
    Scene* scene = malloc(sizeof(Scene));
    Scene_DefaultInit(scene, SCENE_TYPE_MAIN_CHAR_EYE, parent, name);

    MainCharEyeSceneData* sceneData = malloc(sizeof(MainCharEyeSceneData));
    sceneData->eyelidHeight = 0.0f;
    sceneData->eyeTexture = Graphics_LoadTexture("res/images/main_char/MainCharEye.png");
    sceneData->blinkTween = NULL;

    scene->sceneData = sceneData;
    scene->transform.origin = (Vector2){ .x = sceneData->eyeTexture.width * 0.5f, .y = sceneData->eyeTexture.height * 0.5f };

    scene->drawFunction = Draw;
    scene->cleanupFunction = Cleanup;

    return scene;
}

void MainCharEyeScene_Blink(Scene* scene)
{
    ASSERT_SCENE_TYPE(scene, SCENE_TYPE_MAIN_CHAR_EYE);
    MainCharEyeSceneData* sceneData = scene->sceneData;

    if (sceneData->blinkTween) return;

    sceneData->blinkTween = Tween_CreateFunction(c_blinkAnimationLength, scene, BlinkAnimationTweenFunction, TWEEN_INTERPOLATION_QUADRATIC);
    Tween_SetOnFinishCallback(sceneData->blinkTween, scene, OnBlinkAnimationFinished);
}
