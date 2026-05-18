#include "main_char_bullet_scene.h"
#include "graphics/draw_order.h"
#include "graphics/graphics.h"
#include "logging.h"
#include "physics/collider.h"
#include "physics/raycast.h"
#include "physics/transform.h"
#include "point_light_scene.h"
#include "random.h"
#include "result.h"
#include "scene.h"
#include "scene/scene.h"
#include "scene/sprite_scene.h"
#include "scene_type.h"
#include "tween.h"
#include "vector2.h"
#include "physics/collision_layer.h"

#include <stdlib.h>

#define SCENE_TYPE SCENE_TYPE_MAIN_CHAR_BULLET

typedef struct MainCharBulletSceneData {
    Vector2 speed;
    float damage;
    float trimDistanceX;
    float trimDistanceY;
} MainCharBulletSceneData;

typedef struct MainCharBulletSceneStaticData {
    bool initialized;
    Texture2D sparkTexture;
} MainCharBulletSceneStaticData;

static MainCharBulletSceneStaticData s_staticData;

constexpr float c_defaultDamage = 1.0f;
constexpr double c_sparkLifetime = 0.05f;
constexpr Vector2 c_size = (Vector2){ .x = 8.0f, 2.0f };

static void TweenOnProjectileSparkFinishCallback(Scene* scene)
{
    Scene_QueueFree(scene);
}

static void Update(Scene* scene, double deltatime)
{
    MainCharBulletSceneData* sceneData = scene->sceneData;

    float raycastLenght = c_size.x + Vector2_Length(sceneData->speed) * deltatime;
    Raycast raycast = Raycast_New(scene->globalTransform.position, Transform_Forward(&scene->globalTransform), raycastLenght, COLLISION_LAYER_WORLD | COLLISION_LAYER_ENEMY);

    Vector2 collisionPoint;
    const Collider* collider = Raycast_CheckForCollision(&raycast, &collisionPoint);
    if (collider)
    {
        Scene_QueueFree(scene);

        if (collider->collisionCallback)
            collider->collisionCallback(collider->damageCallbackOwner, (CollisionCallbackInfo){ .damage = sceneData->damage });

        Scene* sparkScene = SpriteScene_CreateWithTexture(Scene_GetRoot(scene), &s_staticData.sparkTexture, "Projectile Spark Srpite");
        sparkScene->transform.position = collisionPoint;
        sparkScene->transform.rotation = RandomFloat() * 360.0f;
        sparkScene->transform.origin = (Vector2){ .x = s_staticData.sparkTexture.width * 0.5f, .y = s_staticData.sparkTexture.height * 0.5f };
        sparkScene->transform.scale = Vector2_Uniform(RandomFloat() + 0.5f);

        constexpr Color sparkLightColor = { .r = 252u, .g = 229u, .b = 150u, .a = 30u };
        float sparkLightRadius = RandomFloat() * 50.0f + 20.0f;
        PointLightScene_Create(sparkScene, "MainChar Bullet Spark Light", sparkLightColor, sparkLightRadius);

        Tween_CreateTimer(c_sparkLifetime, sparkScene, TweenOnProjectileSparkFinishCallback);
        return;
    }

    if (scene->globalTransform.position.x > sceneData->trimDistanceX ||
        scene->globalTransform.position.y > sceneData->trimDistanceY ||
        scene->globalTransform.position.x < 0.0f ||
        scene->globalTransform.position.y < 0.0f)
    {
        Scene_QueueFree(scene);
    }

    scene->transform.position.x += sceneData->speed.x * deltatime;
    scene->transform.position.y += sceneData->speed.y * deltatime;
    
    // Draw raycast
    //Graphics_DrawVectorFromPoint(scene->globalTransform.position, Vector2_MultScalar(raycast.direction, raycast.length), COLOR_GREEN);
}

static void Draw(Scene* scene)
{
    Graphics_SetModelMatrix(&scene->globalTransform);
    Graphics_DrawRectT(c_size, COLOR_YELLOW, DRAW_ORDER_DEFAULT);
    Graphics_ClearModelMatrix();

    constexpr Color lightColor = (Color) { .r = 235u, .g = 190u, .b = 235u, .a = 10u };
    constexpr float lightRadius = 30.0f;
    Graphics_DrawLight(scene->globalTransform.position, lightRadius, lightColor);
}

Scene* MainCharBulletScene_Create(Scene* parent, Vector2 initialPosition, Vector2 speed)
{
    if (!s_staticData.initialized)
    {
        const char* sparkImagePath = "res/images/ProjectileSpark.png";
        s_staticData.sparkTexture = Graphics_LoadTexture(sparkImagePath);
        if (s_staticData.sparkTexture.id == 0)
            PANIC_EX(LogErrorM("Failed to load texture. Path: %s", sparkImagePath););

        s_staticData.initialized = true;
    }

    Scene* scene = malloc(sizeof(Scene));
    Scene_DefaultInit(scene, SCENE_TYPE_MAIN_CHAR_BULLET, parent, "Main Char Bullet");
    scene->transform.rotation = Vector2_Angle(speed);
    scene->transform.position = initialPosition;
    MainCharBulletSceneData* sceneData = malloc(sizeof(MainCharBulletSceneData));
    sceneData->damage = c_defaultDamage;
    sceneData->speed = speed;
    sceneData->trimDistanceX = Graphics_GetScreenWidth();
    sceneData->trimDistanceY = Graphics_GetScreenHeight();
    scene->sceneData = sceneData;

    scene->updateFunction = Update;
    scene->drawFunction = Draw;

    return scene;
}
