#include "main_char_gun_scene.h"
#include "graphics/graphics.h"
#include "graphics/image.h"
#include "main_char_bullet_scene.h"
#include "scene/scene.h"
#include "scene_type.h"
#include "vector2.h"
#include <math.h>
#include <stdlib.h>

typedef struct MainCharGunSceneData {
    Texture2D gunTexture;
    Texture2D gunTextureFlippedY;
    bool flipped;
    double elapsedSinceShot;
} MainCharGunSceneData;

constexpr float c_handSize = 4.0f;
constexpr float c_bulletSpeed = 1000.0f;
constexpr double c_shootDelay = 0.1f;

static void Update(Scene* scene, double deltatime)
{
    MainCharGunSceneData* sceneData = (MainCharGunSceneData*)scene->sceneData;

    sceneData->flipped = fabs(scene->globalTransform.rotation) > 90.0f;
    sceneData->elapsedSinceShot += deltatime;
}

static void Draw(Scene* scene)
{
    constexpr Color handColor = (Color) { .r = 150, .g = 150, .b = 120, .a = 255 };
    constexpr float leftHandOffsetX = 3.0f;
    constexpr float leftHandOffsetY = -3.0f;
    constexpr float rightHandOffsetX = 20.0f;
    constexpr float rightHandOffsetY = -3.0f;

    MainCharGunSceneData* sceneData = (MainCharGunSceneData*)scene->sceneData;

    Vector2 leftHandOffset = (Vector2){ .x = leftHandOffsetX, .y = sceneData->flipped ? -leftHandOffsetY : leftHandOffsetY };
    Vector2 rightHandOffset = (Vector2){ .x = rightHandOffsetX, .y = sceneData->flipped ? -rightHandOffsetY : rightHandOffsetY };

    Vector2 leftHandPosition = Vector2_Add(scene->globalTransform.position, Vector2_Rotate(leftHandOffset, scene->globalTransform.rotation));
    Vector2 rightHandPosition = Vector2_Add(scene->globalTransform.position, Vector2_Rotate(rightHandOffset, scene->globalTransform.rotation));

    Graphics_SetTransformW(&scene->globalTransform);
    Graphics_DrawTextureT(sceneData->flipped ? &sceneData->gunTextureFlippedY : &sceneData->gunTexture, DRAW_ORDER_MAIN_CHAR);
    Graphics_DrawCircleW(leftHandPosition, c_handSize, handColor, DRAW_ORDER_MAIN_CHAR);
    Graphics_DrawCircleW(rightHandPosition, c_handSize, handColor, DRAW_ORDER_MAIN_CHAR);
    Graphics_ClearTransform();
}

static void Cleanup(Scene* scene)
{
    MainCharGunSceneData* sceneData = (MainCharGunSceneData*)scene->sceneData;
    Graphics_UnloadTexture(sceneData->gunTexture);
}

Scene* MainCharGunScene_Create(Scene* parent)
{
    Scene* scene = malloc(sizeof(Scene));
    Scene_DefaultInit(scene, SCENE_TYPE_MAIN_CHAR_GUN, "Main Char Gun");
    MainCharGunSceneData* sceneData = malloc(sizeof(MainCharGunSceneData));

    Image gunImage = Image_Load("res/images/main_char/MainCharGun.png");
    sceneData->gunTexture = Graphics_LoadTextureFromImage(&gunImage);
    Image_FlipVertical(&gunImage);
    sceneData->gunTextureFlippedY = Graphics_LoadTextureFromImage(&gunImage);
    Image_Free(&gunImage);
    sceneData->elapsedSinceShot = 0.0;

    scene->sceneData = sceneData;

    Scene_AddChild(parent, scene);
    scene->transform.origin = (Vector2){ .x = 0.0f, .y = sceneData->gunTexture.height * 0.5f };

    scene->updateFunction = Update;
    scene->drawFunction = Draw;
    scene->cleanupFunction = Cleanup;

    return scene;
}

void MainCharGunScene_Shoot(Scene* scene)
{
    ASSERT_SCENE_TYPE(scene, SCENE_TYPE_MAIN_CHAR_GUN);

    MainCharGunSceneData* sceneData = (MainCharGunSceneData*)scene->sceneData;

    if (sceneData->elapsedSinceShot < c_shootDelay) return;

    float flippedFactor = sceneData->flipped ? -1.0f : 1.0f;
    Vector2 gunVector = Vector2_Rotate((Vector2){ .x = sceneData->gunTexture.width, .y = sceneData->gunTexture.height * 0.25f * flippedFactor }, scene->globalTransform.rotation);
    Vector2 bulletPosition = Vector2_Add(scene->globalTransform.position, gunVector);
    
    Vector2 bulletSpeed = Vector2_MultScalar(Vector2_Rotate(Vector2_Right(), scene->globalTransform.rotation), c_bulletSpeed);

    MainCharBulletScene_Create(Scene_GetRoot(scene), bulletPosition, bulletSpeed);

    sceneData->elapsedSinceShot = 0.0;
}
