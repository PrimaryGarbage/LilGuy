#include "main_char_jetpack_fire_scene.h"
#include "animated_sprite_scene.h"
#include "graphics/draw_order.h"
#include "scene/scene.h"
#include "scene_type.h"
#include "tween.h"
#include <stdlib.h>

typedef struct MainCharJetpackFireSceneData {
    float opacity;
    bool inAnimation;
    Scene* animatedFireScene;
} MainCharJetpackFireSceneData;

constexpr double c_animationLength = 0.2f;

static void Draw(Scene* scene)
{
    MainCharJetpackFireSceneData* sceneData = scene->sceneData;

    u8 opacity = sceneData->opacity * 255.0f;
    AnimatedSpriteScene_SetTint(sceneData->animatedFireScene, (Color){ .r = 255, .g = 255, .b = 255, .a = opacity});

    // Origin
    //Graphics_DrawCircleW(scene->globalTransform.position, 2.0f, COLOR_WHITE, DRAW_ORDER_TOP);
}

Scene* MainCharJetpackFireScene_Create(Scene* parent)
{
    Scene* scene = malloc(sizeof(Scene));
    Scene_DefaultInit(scene, SCENE_TYPE_MAIN_CHAR_JETPACK_FIRE, parent, "Main Char Jetpack Fire");

    MainCharJetpackFireSceneData* sceneData = malloc(sizeof(MainCharJetpackFireSceneData));

    constexpr double animatedSpriteSwitchTime = 0.05;

    Scene* animatedFireScene = AnimatedSpriteScene_Create(scene, "res/images/main_char/MainCharJetpackFire.png", 7u, 1u, animatedSpriteSwitchTime, 
        DRAW_ORDER_MAIN_CHAR_JETPACK, "Main Char Jetpack Animated Sprite");
    animatedFireScene->transform.origin = (Vector2){ 
        .x = AnimatedSpriteScene_GetSpriteSize(animatedFireScene).x * 0.5f, 
        .y = 0.0f };
    sceneData->opacity = 1.0f;
    sceneData->inAnimation = false;
    sceneData->animatedFireScene = animatedFireScene;

    scene->sceneData = sceneData;

    scene->drawFunction = Draw;

    return scene;
};

static void OnHideAnimationFinish(Scene* scene)
{
    MainCharJetpackFireSceneData* sceneData = scene->sceneData;
    sceneData->opacity = 0.0f;
    scene->transform.scale = (Vector2){ .x = 1.0f, .y = 0.0f };
    sceneData->inAnimation = false;
}

static void OnShowAnimationFinish(Scene* scene)
{
    MainCharJetpackFireSceneData* sceneData = scene->sceneData;
    sceneData->opacity = 1.0f;
    scene->transform.scale = Vector2_One();
    sceneData->inAnimation = false;
}

static void ShowTweenAnimationFunction(Scene* scene, double weight, double _)
{
    MainCharJetpackFireSceneData* sceneData = scene->sceneData;

    sceneData->opacity = weight;
    //scene->transform.scale.x = weight;
    scene->transform.scale.y = weight;
}

static void HideTweenAnimationFunction(Scene* scene, double weight, double _)
{
    MainCharJetpackFireSceneData* sceneData = scene->sceneData;

    sceneData->opacity = 1.0f - weight;
    //scene->transform.scale.x = 1.0 - weight;
    scene->transform.scale.y = 1.0f - weight;
}

void MainCharJetpackFireScene_Show(Scene* scene)
{
    ASSERT_SCENE_TYPE(scene, SCENE_TYPE_MAIN_CHAR_JETPACK_FIRE);

    MainCharJetpackFireSceneData* sceneData = scene->sceneData;
    if (sceneData->inAnimation || sceneData->opacity == 1.0f) return;
    sceneData->inAnimation = true;

    TweenHandle tween = Tween_CreateFunction(c_animationLength, scene, ShowTweenAnimationFunction, TWEEN_INTERPOLATION_LINEAR);
    Tween_SetOnFinishCallback(tween, scene, OnShowAnimationFinish);
}

void MainCharJetpackFireScene_Hide(Scene* scene)
{
    ASSERT_SCENE_TYPE(scene, SCENE_TYPE_MAIN_CHAR_JETPACK_FIRE);

    MainCharJetpackFireSceneData* sceneData = scene->sceneData;
    if (sceneData->inAnimation || sceneData->opacity == 0.0f) return;
    sceneData->inAnimation = true;

    TweenHandle tween = Tween_CreateFunction(c_animationLength, scene, HideTweenAnimationFunction, TWEEN_INTERPOLATION_LINEAR);
    Tween_SetOnFinishCallback(tween, scene, OnHideAnimationFinish);
}
