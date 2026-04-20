#include "main_char_gun_scene.h"
#include "graphics/draw_order.h"
#include "graphics/graphics.h"
#include "graphics/image.h"
#include "main_char_bullet_scene.h"
#include "scene/scene.h"
#include "scene_type.h"
#include "sprite_scene.h"
#include "tween.h"
#include "vector2.h"
#include <math.h>
#include <stdlib.h>

typedef struct MainCharGunSceneData {
    Texture2D gunTexture;
    Texture2D gunTextureFlippedY;
    Scene* gunFlash;
    bool gunFlipped;
    double elapsedSinceShot;
} MainCharGunSceneData;

constexpr float c_handSize = 4.0f;
constexpr float c_bulletSpeed = 1000.0f;
constexpr double c_shootDelay = 0.1f;
constexpr double c_flashLifetime = 0.1f;

static void OnGunFlip(Scene* scene)
{
    MainCharGunSceneData* sceneData = scene->sceneData;

}

static void Update(Scene* scene, double deltatime)
{
    MainCharGunSceneData* sceneData = scene->sceneData;

    sceneData->gunFlipped = fabsf(scene->globalTransform.rotation) > 90.0f;

    sceneData->gunFlash->transform.position.y = sceneData->gunFlipped ? -2.0f : -(float)sceneData->gunTexture.height * 0.5f - 2.0f;

    sceneData->elapsedSinceShot += deltatime;
}

static void Draw(Scene* scene)
{
    constexpr Color handColor = (Color) { .r = 150, .g = 150, .b = 120, .a = 255 };
    constexpr float leftHandOffsetX = 5.0f;
    constexpr float leftHandOffsetY = 4.0f;
    constexpr float rightHandOffsetX = 20.0f;
    constexpr float rightHandOffsetY = 4.0f;

    MainCharGunSceneData* sceneData = scene->sceneData;

    Vector2 leftHandOffset = (Vector2){ .x = leftHandOffsetX, .y = sceneData->gunFlipped ? -leftHandOffsetY : leftHandOffsetY };
    Vector2 rightHandOffset = (Vector2){ .x = rightHandOffsetX, .y = sceneData->gunFlipped ? -rightHandOffsetY : rightHandOffsetY };

    Vector2 leftHandPosition = Vector2_Add(scene->globalTransform.position, Vector2_Rotate(leftHandOffset, scene->globalTransform.rotation));
    Vector2 rightHandPosition = Vector2_Add(scene->globalTransform.position, Vector2_Rotate(rightHandOffset, scene->globalTransform.rotation));

    Graphics_SetModelMatrix(&scene->globalTransform);
    Graphics_DrawTextureT(sceneData->gunFlipped ? &sceneData->gunTextureFlippedY : &sceneData->gunTexture, DRAW_ORDER_MAIN_CHAR, COLOR_WHITE);
    Graphics_DrawCircle(leftHandPosition, c_handSize, handColor, DRAW_ORDER_MAIN_CHAR);
    Graphics_DrawCircle(rightHandPosition, c_handSize, handColor, DRAW_ORDER_MAIN_CHAR);
    Graphics_ClearModelMatrix();
}

static void Cleanup(Scene* scene)
{
    MainCharGunSceneData* sceneData = scene->sceneData;
    Graphics_UnloadTexture(sceneData->gunTexture);
}

Scene* MainCharGunScene_Create(Scene* parent)
{
    Scene* scene = malloc(sizeof(Scene));
    Scene_DefaultInit(scene, SCENE_TYPE_MAIN_CHAR_GUN, "Main Char Gun");
    if (parent) Scene_AddChild(parent, scene);
    MainCharGunSceneData* sceneData = malloc(sizeof(MainCharGunSceneData));

    Image gunImage = Image_Load("res/images/main_char/MainCharGun.png");
    sceneData->gunTexture = Graphics_LoadTextureFromImage(&gunImage);
    Image_FlipVertical(&gunImage);
    sceneData->gunTextureFlippedY = Graphics_LoadTextureFromImage(&gunImage);
    Image_Free(&gunImage);
    sceneData->elapsedSinceShot = 0.0;

    scene->sceneData = sceneData;

    scene->transform.origin = (Vector2){ .x = 0.0f, .y = sceneData->gunTexture.height * 0.5f };

    scene->updateFunction = Update;
    scene->drawFunction = Draw;
    scene->cleanupFunction = Cleanup;

    Scene* gunFlash = SpriteScene_Create(scene, "res/images/main_char/MainCharGunFlash.png", "Main Char Gun Flash");
    SpriteScene_SetDrawOrder(gunFlash, DRAW_ORDER_MAIN_CHAR);
    gunFlash->transform.position = (Vector2){ .x = sceneData->gunTexture.width, .y = -(float)sceneData->gunTexture.height * 0.5f - 2.0f};
    gunFlash->visible = false;
    sceneData->gunFlash = gunFlash;

    return scene;
}

void TweenAnimationFlashCallback(Scene* scene)
{
    MainCharGunSceneData* sceneData = (MainCharGunSceneData*)scene->sceneData;
    sceneData->gunFlash->visible = false;
}

void MainCharGunScene_Shoot(Scene* scene)
{
    ASSERT_SCENE_TYPE(scene, SCENE_TYPE_MAIN_CHAR_GUN);

    MainCharGunSceneData* sceneData = (MainCharGunSceneData*)scene->sceneData;

    if (sceneData->elapsedSinceShot < c_shootDelay) return;

    float flippedFactor = sceneData->gunFlipped ? -1.0f : 1.0f;
    Vector2 gunVector = (Vector2){ .x = sceneData->gunTexture.width, .y = -(float)sceneData->gunTexture.height * 0.25f * flippedFactor };
    gunVector = Vector2_Rotate(gunVector, scene->globalTransform.rotation);
    Vector2 bulletPosition = Vector2_Add(scene->globalTransform.position, gunVector);
    
    Vector2 bulletSpeed = Vector2_MultScalar(Vector2_Rotate(Vector2_Right(), scene->globalTransform.rotation), c_bulletSpeed);

    MainCharBulletScene_Create(Scene_GetRoot(scene), bulletPosition, bulletSpeed);

    sceneData->elapsedSinceShot = 0.0;

    sceneData->gunFlash->visible = true;
    Tween_CreateTimer(c_flashLifetime, scene, TweenAnimationFlashCallback);
}
