#include "main_char_bullet_scene.h"
#include "graphics/graphics.h"
#include "scene.h"
#include "scene_type.h"
#include "vector2.h"
#include <stdlib.h>

typedef struct MainCharBulletSceneData {
    Vector2 speed;
    float damage;
    float trimDistanceX;
    float trimDistanceY;
} MainCharBulletSceneData;

constexpr float c_defaultDamage = 1.0f;
constexpr float c_size = 2.0f;

static void Update(Scene* scene, double deltatime)
{
    MainCharBulletSceneData* sceneData = scene->sceneData;

    if (scene->globalTransform.position.x > sceneData->trimDistanceX ||
        scene->globalTransform.position.y > sceneData->trimDistanceY ||
        scene->globalTransform.position.x < 0.0f ||
        scene->globalTransform.position.y < 0.0f)
    {
        Scene_QueueFree(scene);
    }

    scene->transform.position.x += sceneData->speed.x * deltatime;
    scene->transform.position.y += sceneData->speed.y * deltatime;

}

static void Draw(Scene* scene)
{
    Graphics_SetTransformW(&scene->globalTransform);
    Graphics_DrawCircleT(c_size, COLOR_YELLOW, DRAW_ORDER_DEFAULT);
    Graphics_ClearTransform();
}

Scene* MainCharBulletScene_Create(Scene* parent, Vector2 initialPosition, Vector2 speed)
{
    Scene* scene = malloc(sizeof(Scene));
    Scene_DefaultInit(scene, SCENE_TYPE_MAIN_CHAR_BULLET, "Main Char Bullet");
    Scene_AddChild(parent, scene);
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
