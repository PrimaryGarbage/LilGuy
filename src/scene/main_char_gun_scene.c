#include "main_char_gun_scene.h"
#include "graphics/graphics.h"
#include "graphics/image.h"
#include "scene/scene.h"
#include <math.h>
#include <stdlib.h>

typedef struct MainCharGunSceneData {
    Texture2D gunTexture;
    Texture2D gunTextureFlippedY;
} MainCharGunSceneData;

constexpr float c_handSize = 4.0f;

static void Draw(Scene* scene)
{
    constexpr Color handColor = (Color) { .r = 150, .g = 150, .b = 120, .a = 255 };
    constexpr float leftHandOffsetX = 3.0f;
    constexpr float leftHandOffsetY = -3.0f;
    constexpr float rightHandOffsetX = 20.0f;
    constexpr float rightHandOffsetY = -3.0f;

    MainCharGunSceneData* sceneData = (MainCharGunSceneData*)scene->sceneData;

    bool flipped = fabs(scene->globalTransform.rotation) > 90.0f;

    Vector2 leftHandOffset = (Vector2){ .x = leftHandOffsetX, .y = flipped ? -leftHandOffsetY : leftHandOffsetY };
    Vector2 rightHandOffset = (Vector2){ .x = rightHandOffsetX, .y = flipped ? -rightHandOffsetY : rightHandOffsetY };

    Vector2 leftHandPosition = Vector2_Add(scene->globalTransform.position, Vector2_Rotate(leftHandOffset, scene->globalTransform.rotation));
    Vector2 rightHandPosition = Vector2_Add(scene->globalTransform.position, Vector2_Rotate(rightHandOffset, scene->globalTransform.rotation));

    Graphics_SetTransformW(&scene->globalTransform);
    Graphics_DrawTextureT(flipped ? &sceneData->gunTextureFlippedY : &sceneData->gunTexture, DRAW_ORDER_MAIN_CHAR);
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

    scene->sceneData = sceneData;

    scene->parent = parent;
    scene->transform.origin = (Vector2){ .x = 0.0f, .y = sceneData->gunTexture.height * 0.5f };

    scene->drawFunction = Draw;
    scene->cleanupFunction = Cleanup;

    return scene;
}
