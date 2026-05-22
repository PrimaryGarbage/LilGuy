#include "tumor_scene.h"
#include "graphics/color.h"
#include "graphics/draw_order.h"
#include "graphics/graphics.h"
#include "scene/animated_sprite_scene.h"
#include "scene/collider_scene.h"
#include "scene/scene.h"
#include "scene_type.h"
#include "spark_scene.h"
#include "splash_scene.h"
#include "vector2.h"
#include "physics/collision_layer.h"
#include "animated_sprite_scene.h"

#include <stdlib.h>

#define SCENE_TYPE SCENE_TYPE_TUMOR

typedef struct TumorSceneData {
    Scene* sprite;
    Scene* mainChar;
    Vector2 speed;
    double animationElapsed;
    float health;
} TumorSceneData;

constexpr float c_acceleration = 200.0f;
constexpr float c_maxSpeed = 150.0f;
constexpr float c_maxHealth = 10.0f;
constexpr float c_damage = 10.0f;
constexpr float c_attackRecoilStrength = 100.0f;

static void Update(Scene* scene, double deltatime)
{
    TumorSceneData* sceneData = scene->sceneData;
    sceneData->animationElapsed += deltatime;

    if (sceneData->health <= 0.0f)
    {
        Scene_QueueFree(scene);
        SplashScene_Create(Scene_GetRoot(), scene->globalTransform.position, 1.5f, "Tumor Death Splash", COLOR_RED);
        return;
    }

    Vector2 direction = Vector2_Normalize(Vector2_Sub(sceneData->mainChar->globalTransform.position, scene->globalTransform.position));
    sceneData->speed = Vector2_Clamp(-c_maxSpeed, c_maxSpeed, Vector2_Add(sceneData->speed, Vector2_MultScalar(direction, c_acceleration * deltatime)));
    //sceneData->speed = Vector2_Add(sceneData->speed, Vector2_MultScalar(direction, c_acceleration * deltatime));
    scene->transform.position = Vector2_Add(scene->transform.position, Vector2_MultScalar(sceneData->speed, deltatime));
}

static void Draw(Scene* scene)
{
    //TumorSceneData* sceneData = scene->sceneData;

    // Origin
    //Graphics_DrawCircle(scene->globalTransform.position, 2.0f, COLOR_WHITE, DRAW_ORDER_TOP);
}

static void OnCollision(Scene* scene, ColliderScene_CollisionInfo info)
{
    if (info.collider->owner && info.collider->owner->takeDamageFunction)
    {
        TumorSceneData* sceneData = scene->sceneData;

        Vector2 collisionPoint = Rect_GetCenter(&info.collisionRect);
        info.collider->owner->takeDamageFunction(info.collider->owner, c_damage, collisionPoint);
        Vector2 recoilDirection = Vector2_Normalize(Vector2_Sub(scene->globalTransform.position, collisionPoint));
        sceneData->speed = Vector2_Add(sceneData->speed, Vector2_MultScalar(recoilDirection, c_attackRecoilStrength));
    }
}

static void TakeDamage(Scene* scene, float damage, Vector2 collisionPoint)
{
    ASSERT_SCENE_TYPE(scene);

    TumorSceneData* sceneData = scene->sceneData;
    sceneData->health -= damage;
    SparkScene_Create(Scene_GetRoot(), collisionPoint, 2.0f, COLOR_RED, false, COLOR_WHITE, "Tumor Collision Spark");
}

Scene* TumorScene_Create(Scene* parent, Scene* mainChar)
{
    Scene* scene = malloc(sizeof(Scene));
    Scene_DefaultInit(scene, SCENE_TYPE_TUMOR, parent, "Tumor");
    TumorSceneData* sceneData = malloc(sizeof(TumorSceneData));
    scene->sceneData = sceneData;

    constexpr double animatedSpriteSwitchTime = 0.2f;
    sceneData->sprite = AnimatedSpriteScene_Create(scene, "res/images/Tumor.png", 4u, 1u, animatedSpriteSwitchTime, DRAW_ORDER_NPC, "Tumor Animated Sprite");
    sceneData->mainChar = mainChar;
    sceneData->animationElapsed = 0.0;
    sceneData->health = c_maxHealth;
    sceneData->speed = Vector2_Zero();

    Vector2 spriteSize = AnimatedSpriteScene_GetSpriteSize(sceneData->sprite);
    Vector2 colliderPosition = { .x = -spriteSize.x * 0.5f, .y = -spriteSize.y * 0.5f };

    Scene* collider = ColliderScene_Create(scene, scene, spriteSize, "Tumor Collider");
    collider->transform.position = colliderPosition;
    ColliderScene_SetCollisionLayers(collider, COLLISION_LAYER_ENEMY);
    ColliderScene_SetCollisionScan(collider, COLLISION_LAYER_MAIN_CHAR);
    ColliderScene_SetVisible(collider, false);
    ColliderScene_SetOnCollisionCallback(collider, scene, OnCollision);

    sceneData->sprite->transform.origin = Vector2_MultScalar(spriteSize, 0.5f);

    scene->updateFunction = Update;
    scene->drawFunction = Draw;
    scene->takeDamageFunction = TakeDamage;

    return scene;
}