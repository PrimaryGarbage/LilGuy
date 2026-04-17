#include "graphics/graphics.h"
#include "scene.h"
#include "scene_type.h"
#include "tween.h"
#include "vector2.h"
#include "main_char_eye_scene.h"
#include <stdlib.h>

constexpr Vector2 c_eyeSize = { 3.0f, 6.0f };
constexpr double c_blinkAnimationLength = 0.1f;

typedef struct MainCharEyeSceneData {
    float eyelidHeight;
} MainCharEyeSceneData;

static void BlinkAnimationTweenFunction(Scene* scene, double elapsed)
{
    MainCharEyeSceneData* sceneData = scene->sceneData;

    float weight = elapsed / c_blinkAnimationLength;
    float offsetWeight = weight < 0.5f ? weight * 2.0f : 2.0f - weight * 2.0f;

    sceneData->eyelidHeight = c_eyeSize.y * offsetWeight;
}

static void OnBlinkAnimationFinished(Scene* scene)
{
    MainCharEyeSceneData* sceneData = scene->sceneData;
    sceneData->eyelidHeight = 0.0f;
}

static void Draw(Scene* scene)
{
    MainCharEyeSceneData* sceneData = scene->sceneData;

    Graphics_SetTransformW(&scene->globalTransform);
    Graphics_DrawRectT(c_eyeSize, COLOR_WHITE);
    Graphics_DrawRectT((Vector2) { c_eyeSize.x, sceneData->eyelidHeight }, COLOR_BLACK);
    Graphics_ClearTransform();
}

Scene* MainCharEyeScene_Create(Scene* parent)
{
    Scene* scene = malloc(sizeof(Scene));
    Scene_DefaultInit(scene, SCENE_TYPE_MAIN_CHAR_EYE, "Main Char Eye Scene");

    MainCharEyeSceneData* sceneData = malloc(sizeof(MainCharEyeSceneData));
    sceneData->eyelidHeight = 0.0f;

    scene->sceneData = sceneData;
    scene->parent = parent;
    scene->transform.origin = Vector2_MultScalar(c_eyeSize, 0.5f);

    scene->drawFunction = Draw;

    return scene;
}

void MainCharEyeScene_Blink(Scene* scene)
{
    ASSERT_SCENE_TYPE(scene, SCENE_TYPE_MAIN_CHAR_EYE);

    TweenHandle tween = Tween_CreateFunction(c_blinkAnimationLength, scene, BlinkAnimationTweenFunction);
    Tween_SetOnFinishCallback(tween, scene, OnBlinkAnimationFinished);
}
