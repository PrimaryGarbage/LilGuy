#include "main_char_jetpack_fire_scene.h"
#include "graphics/graphics.h"
#include "graphics/texture2d.h"
#include "scene/scene.h"
#include "scene_type.h"
#include "tween.h"
#include <stdlib.h>

typedef struct MainCharJetpackFireSceneData {
    Texture2D fireTexture;
    float opacity;
    bool inAnimation;
} MainCharJetpackFireSceneData;

constexpr float c_animationLength = 0.2f;

static void Draw(Scene* scene)
{
    MainCharJetpackFireSceneData* sceneData = scene->sceneData;

    Graphics_SetTransformW(&scene->globalTransform);
    Graphics_DrawTintedTextureT(&sceneData->fireTexture, (Color){ .r = 255, .g = 255, .b = 255, .a = (u8)(sceneData->opacity * 255.0f)});
    Graphics_ClearTransform();

    Graphics_DrawCircleW(scene->globalTransform.position, 2.0f, COLOR_WHITE);
}

static void Cleanup(Scene* scene)
{
    MainCharJetpackFireSceneData* sceneData = scene->sceneData;

    Graphics_UnloadTexture(sceneData->fireTexture);
}

Scene* MainCharJetpackFireScene_Create(Scene* parent)
{
    Scene* scene = malloc(sizeof(Scene));
    Scene_DefaultInit(scene, SCENE_TYPE_MAIN_CHAR_JETPACK_FIRE, "Main Char Jetpack Fire");
    scene->parent = parent;

    MainCharJetpackFireSceneData* sceneData = malloc(sizeof(MainCharJetpackFireSceneData));

    sceneData->fireTexture = Graphics_LoadTexture("res/images/main_char/MainCharJetpackFire.png");
    sceneData->opacity = 1.0f;
    sceneData->inAnimation = false;

    scene->sceneData = sceneData;

    scene->transform.origin = (Vector2){ .x = sceneData->fireTexture.width * 0.5f, sceneData->fireTexture.height }; 

    scene->drawFunction = Draw;
    scene->cleanupFunction = Cleanup;

    return scene;
};

static void OnHideAnimationFinish(Scene* scene)
{
    MainCharJetpackFireSceneData* sceneData = scene->sceneData;
    sceneData->opacity = 0.0f;
    scene->transform.scale = Vector2_Zero();
    sceneData->inAnimation = false;
}

static void OnShowAnimationFinish(Scene* scene)
{
    MainCharJetpackFireSceneData* sceneData = scene->sceneData;
    sceneData->opacity = 1.0f;
    scene->transform.scale = Vector2_One();
    sceneData->inAnimation = false;
}

static void ShowTweenAnimationFunction(Scene* scene, double elapsed)
{
    MainCharJetpackFireSceneData* sceneData = scene->sceneData;

    float weight = elapsed / c_animationLength;
    sceneData->opacity = weight;
    scene->transform.scale.x = weight;
    scene->transform.scale.y = weight;
}

static void HideTweenAnimationFunction(Scene* scene, double elapsed)
{
    MainCharJetpackFireSceneData* sceneData = scene->sceneData;

    float weight = elapsed / c_animationLength;
    sceneData->opacity = 1.0f - weight;
    scene->transform.scale.x = 1.0 - weight;
    scene->transform.scale.y = 1.0 - weight;
}

void MainCharJetpackFireScene_Show(Scene* scene)
{
    ASSERT_SCENE_TYPE(scene, SCENE_TYPE_MAIN_CHAR_JETPACK_FIRE);

    MainCharJetpackFireSceneData* sceneData = scene->sceneData;
    if (sceneData->inAnimation || sceneData->opacity == 1.0f) return;
    sceneData->inAnimation = true;

    TweenHandle tween = Tween_CreateFunction(c_animationLength, scene, ShowTweenAnimationFunction);
    Tween_SetOnFinishCallback(tween, scene, OnShowAnimationFinish);
}

void MainCharJetpackFireScene_Hide(Scene* scene)
{
    ASSERT_SCENE_TYPE(scene, SCENE_TYPE_MAIN_CHAR_JETPACK_FIRE);

    MainCharJetpackFireSceneData* sceneData = scene->sceneData;
    if (sceneData->inAnimation || sceneData->opacity == 0.0f) return;
    sceneData->inAnimation = true;

    TweenHandle tween = Tween_CreateFunction(c_animationLength, scene, HideTweenAnimationFunction);
    Tween_SetOnFinishCallback(tween, scene, OnHideAnimationFinish);
}
