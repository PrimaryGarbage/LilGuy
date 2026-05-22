#include "splash_scene.h"
#include "graphics/draw_order.h"
#include "random.h"
#include "scene/animated_sprite_scene.h"
#include "scene/scene.h"
#include "scene_type.h"
#include "vector2.h"
#include <stdlib.h>

typedef struct {
    Color color;
    float size;
    Scene* animatedSpriteScene;
} SplashSceneData;

static void OnAnimationFinish(Scene* scene)
{
    Scene_QueueFree(scene);
}

Scene* SplashScene_Create(Scene* parent, Vector2 position, float scale, const char* name, Color color)
{
    Scene* scene = malloc(sizeof(Scene));
    Scene_DefaultInit(scene, SCENE_TYPE_SPLASH, parent, name);
    SplashSceneData* sceneData = malloc(sizeof(SplashSceneData));
    scene->sceneData = sceneData;

    sceneData->color = color;
    Scene* animatedSprite = AnimatedSpriteScene_Create(scene, "res/images/Splash.png", 5u, 1u, 0.07, DRAW_ORDER_TOP, "Splash Animated Sprite");
    sceneData->animatedSpriteScene = animatedSprite;
    AnimatedSpriteScene_SetTint(animatedSprite, color);
    AnimatedSpriteScene_SetOnAnimationFinishCallback(animatedSprite, OnAnimationFinish, scene);
    animatedSprite->transform.position = position;
    animatedSprite->transform.scale = Vector2_Uniform(scale);
    animatedSprite->transform.rotation = RandomFloat() * 360.0f;
    AnimatedSpriteScene_SetOriginToCenter(animatedSprite);

    return scene;
}
