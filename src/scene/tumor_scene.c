#include "tumor_scene.h"
#include "graphics/draw_order.h"
#include "graphics/graphics.h"
#include "scene/animated_sprite_scene.h"
#include "scene/collider_scene.h"
#include "scene/scene.h"
#include "scene_type.h"
#include "vector2.h"
#include "physics/collision_layer.h"
#include "animated_sprite_scene.h"

#include <stdlib.h>

typedef struct TumorSceneData {
    Scene* sprite;
    Scene* mainChar;
    double animationElapsed;
    float health;
} TumorSceneData;

constexpr float c_maxSpeed = 3.0f;
constexpr float c_maxHealth = 10.0f;

static void Update(Scene* scene, double deltatime)
{
    TumorSceneData* sceneData = scene->sceneData;
    sceneData->animationElapsed += deltatime;

    if (sceneData->health <= 0.0f)
    {
        Scene_QueueFree(scene);
        return;
    }

    Vector2 direction = Vector2_Normalize(Vector2_Sub(sceneData->mainChar->globalTransform.position, scene->globalTransform.position));
    scene->transform.position = Vector2_Add(scene->transform.position, Vector2_MultScalar(direction, c_maxSpeed));
}

static void Draw(Scene* scene)
{
    //TumorSceneData* sceneData = scene->sceneData;

    // Origin
    Graphics_DrawCircle(scene->globalTransform.position, 2.0f, COLOR_WHITE, DRAW_ORDER_TOP);
}

static void TakeDamage(Scene* scene, CollisionCallbackInfo info)
{
    TumorSceneData* sceneData = scene->sceneData;
    sceneData->health -= info.damage;
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

    Vector2 spriteSize = AnimatedSpriteScene_GetSpriteSize(sceneData->sprite);
    Vector2 colliderPosition = { .x = -spriteSize.x * 0.5f, .y = -spriteSize.y * 0.5f };

    Scene* collider = ColliderScene_Create(scene, spriteSize, "Tumor Collider");
    collider->transform.position = colliderPosition;
    ColliderScene_SetCollisionLayers(collider, COLLISION_LAYER_ENEMY);
    ColliderScene_SetCollisionScan(collider, COLLISION_LAYER_ALL);
    ColliderScene_SetVisible(collider, false);
    ColliderScene_SetDamageCallback(collider, TakeDamage, scene);

    sceneData->sprite->transform.origin = Vector2_MultScalar(spriteSize, 0.5f);

    scene->updateFunction = Update;
    scene->drawFunction = Draw;

    return scene;
}
