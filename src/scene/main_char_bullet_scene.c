#include "main_char_bullet_scene.h"
#include "graphics/draw_order.h"
#include "graphics/graphics.h"
#include "physics/raycast.h"
#include "physics/transform.h"
#include "scene.h"
#include "scene_type.h"
#include "vector2.h"
#include <stdlib.h>

typedef struct MainCharBulletSceneData {
    Vector2 speed;
    float damage;
    float trimDistanceX;
    float trimDistanceY;
    Raycast raycast;
} MainCharBulletSceneData;

constexpr float c_defaultDamage = 1.0f;
constexpr Vector2 c_size = (Vector2){ .x = 8.0f, 2.0f };

static void Update(Scene* scene, double deltatime)
{
    MainCharBulletSceneData* sceneData = scene->sceneData;

    sceneData->raycast.length = c_size.x + Vector2_Length(sceneData->speed) * deltatime;
    Vector2 collisionPoint;
    if (Raycast_CheckForCollision(&sceneData->raycast, scene->globalTransform.position, &collisionPoint))
    {
        Scene_QueueFree(scene);
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
    // Graphics_DrawVectorFromPoint(scene->globalTransform.position, Vector2_MultScalar(sceneData->raycast.direction, sceneData->raycast.length * deltatime), COLOR_GREEN);
}

static void Draw(Scene* scene)
{
    Graphics_SetModelMatrix(&scene->globalTransform);
    Graphics_DrawRectT(c_size, COLOR_YELLOW, DRAW_ORDER_DEFAULT);
    Graphics_ClearModelMatrix();

}

Scene* MainCharBulletScene_Create(Scene* parent, Vector2 initialPosition, Vector2 speed)
{
    Scene* scene = malloc(sizeof(Scene));
    Scene_DefaultInit(scene, SCENE_TYPE_MAIN_CHAR_BULLET, parent, "Main Char Bullet");
    scene->transform.rotation = Vector2_Angle(speed);
    scene->transform.position = initialPosition;
    MainCharBulletSceneData* sceneData = malloc(sizeof(MainCharBulletSceneData));
    sceneData->damage = c_defaultDamage;
    sceneData->speed = speed;
    sceneData->trimDistanceX = Graphics_GetScreenWidth();
    sceneData->trimDistanceY = Graphics_GetScreenHeight();
    sceneData->raycast = Raycast_New(speed, 1.0f);
    scene->sceneData = sceneData;

    scene->updateFunction = Update;
    scene->drawFunction = Draw;

    return scene;
}
