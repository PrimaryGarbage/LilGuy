#include "main_char_scene.h"
#include "logging.h"
#include "main_char_gun_scene.h"
#include "main_char_eye_scene.h"
#include "physics/collider.h"
#include "collider_scene.h"
#include "graphics/color.h"
#include "input/input.h"
#include "main_char_foot_scene.h"
#include "math_helpers.h"
#include "random.h"
#include "raycast_scene.h"
#include "scene.h"
#include "scene/scene.h"
#include "scene_type.h"
#include "physics/transform.h"
#include "tween.h"
#include "vector2.h"
#include "graphics/graphics.h"
#include "tween.h"
#include <math.h>

typedef struct MainCharSceneData {
    Vector2 speed;
    bool onGround;
    float fuel;
    Texture2D bodyTexture;
    Scene* leftFoot;
    Scene* rightFoot;
    Scene* leftEye;
    Scene* rightEye;
    Scene* bodyCollider;
    Scene* onGroundRaycast;
    Scene* gun;
    float elapsedSinceLastBlink;
    float bodyToFootMaxDistance;
    bool movingLeftFoot;
    bool movingRightFoot;
} MainCharSceneData;

constexpr float c_maxFuel = 1.0f;
constexpr float c_idleDistanceBetweenFeet = 10.0f;
constexpr double c_landingAnimationLength = 0.2f;

static Vector2 GetNewFootPosition(Scene* scene)
{
    constexpr float c_footStrideX = 20.0f;

    ASSERT_SCENE_TYPE(scene, SCENE_TYPE_MAIN_CHAR);

    MainCharSceneData* sceneData = (MainCharSceneData*)scene->sceneData;

    Vector2 newPos = scene->globalTransform.position;

    if (sceneData->onGround)
    {
        newPos.x += Signf(sceneData->speed.x) * c_footStrideX;
        newPos.y -= sceneData->bodyToFootMaxDistance;
    }
    else
    {
        newPos.x += c_idleDistanceBetweenFeet;
        newPos.y -= sceneData->bodyToFootMaxDistance;
    }

    return newPos;
}

static void Blink(Scene* scene, double deltatime)
{
    constexpr float blinkProbability = 0.05f;

    MainCharSceneData* sceneData = (MainCharSceneData*)scene->sceneData;

    sceneData->elapsedSinceLastBlink += deltatime;

    if (blinkProbability * deltatime * sceneData->elapsedSinceLastBlink > RandomFloat())
    {
        MainCharEyeScene_Blink(sceneData->leftEye);
        MainCharEyeScene_Blink(sceneData->rightEye);
        sceneData->elapsedSinceLastBlink = 0.0f;
    }
}

static void MoveGun(Scene* scene)
{
    constexpr Vector2 positionOffset = (Vector2) { .x = 0.0f, .y = -10.0f };

    MainCharSceneData* sceneData = (MainCharSceneData*)scene->sceneData;

    Vector2 mousePos = Graphics_GetMousePositionW();
    Vector2 dir = Vector2_Sub(mousePos, sceneData->gun->transform.position);
    float angle = Vector2_Angle(dir);

    sceneData->gun->transform.position = Vector2_Lerp(sceneData->gun->transform.position, Vector2_Add(scene->globalTransform.position, positionOffset), 0.5f);
    sceneData->gun->transform.rotation = LerpAnglef(sceneData->gun->transform.rotation, angle, 0.5f);
}

static void MoveFeet(Scene* scene)
{
    constexpr float footMaxDistanceFromOriginX = 30.0f;

    MainCharSceneData* sceneData = (MainCharSceneData*)scene->sceneData;

    if (sceneData->onGround)
    {
        // Check if at least one foot is in valid distance
        // if not, move the foot with bigger distance
        float leftFootDistance = fabsf(sceneData->leftFoot->transform.position.x - scene->globalTransform.position.x);
        float rightFootDistance = fabsf(sceneData->rightFoot->transform.position.x - scene->globalTransform.position.x);

        if (leftFootDistance > footMaxDistanceFromOriginX || rightFootDistance > footMaxDistanceFromOriginX)
        {
            if(leftFootDistance > rightFootDistance)
            {
                if (!MainCharFootScene_InAnimation(sceneData->leftFoot))
                    MainCharFootScene_MoveFoot(sceneData->leftFoot, GetNewFootPosition(scene));
            }
            else
            {
                if (!MainCharFootScene_InAnimation(sceneData->rightFoot))
                    MainCharFootScene_MoveFoot(sceneData->rightFoot, GetNewFootPosition(scene));
            }
        }
    }
    else
    {
        const float lerpWeight = 0.5f;
        sceneData->leftFoot->transform.position.x = Lerpf(sceneData->leftFoot->transform.position.x, scene->globalTransform.position.x - c_idleDistanceBetweenFeet, lerpWeight);
        sceneData->leftFoot->transform.position.y = Lerpf(sceneData->leftFoot->transform.position.y, scene->globalTransform.position.y - sceneData->bodyToFootMaxDistance, lerpWeight);
        sceneData->rightFoot->transform.position.x = Lerpf(sceneData->rightFoot->transform.position.x, scene->globalTransform.position.x + c_idleDistanceBetweenFeet, lerpWeight);
        sceneData->rightFoot->transform.position.y = Lerpf(sceneData->rightFoot->transform.position.y, scene->globalTransform.position.y - sceneData->bodyToFootMaxDistance, lerpWeight);
    }
}

static void LandingTweenAnimationCallback(Scene* scene, double elapsed)
{
    constexpr float animationMaxOffsetY = 10.0f;

    float weight = elapsed / c_landingAnimationLength;
    float offsetWeight = weight < 0.5f ? weight * 2.0f : 2.0f - weight * 2.0f;
    scene->transform.position.y -= animationMaxOffsetY * offsetWeight;
}

static void OnLanding(Scene* scene)
{
    MainCharSceneData* sceneData = (MainCharSceneData*)scene->sceneData;

    sceneData->leftFoot->transform.position.x = scene->globalTransform.position.x - c_idleDistanceBetweenFeet;
    sceneData->leftFoot->transform.position.y = scene->globalTransform.position.y - sceneData->bodyToFootMaxDistance;
    sceneData->rightFoot->transform.position.x = scene->globalTransform.position.x + c_idleDistanceBetweenFeet;
    sceneData->rightFoot->transform.position.y = scene->globalTransform.position.y - sceneData->bodyToFootMaxDistance;

    Tween_CreateFunction(c_landingAnimationLength, scene, LandingTweenAnimationCallback);
}

static void MoveCharacter(Scene* scene, double deltatime)
{
    MainCharSceneData* sceneData = (MainCharSceneData*)scene->sceneData;

    constexpr float moveAccel = 1000.0f;
    constexpr float jumpAccel = 1500.0f;
    constexpr float gravityAccel = 1000.0f;
    constexpr float maxSpeedX = 180.0f;
    constexpr float maxJetpackSpeedX = 400.0f;
    constexpr float maxSpeedY = 1000.0f;
    constexpr float speedDissipationFactor = 0.8f;
    constexpr float fuelBurnRate = 0.4f;
    constexpr float fuelRefillRate = 0.3f;

    sceneData->speed.y -= gravityAccel * deltatime;

    bool tryingToMove = false;

    bool onGround = RaycastScene_CheckForCollision(sceneData->onGroundRaycast, NULL);
    if (!sceneData->onGround && onGround != sceneData->onGround) OnLanding(scene);
    sceneData->onGround = onGround;

    if (Input_IsKeyPressed(INPUT_KEY_A)) {
        tryingToMove = true;
        sceneData->speed.x -= moveAccel * deltatime;
    }
    if (Input_IsKeyPressed(INPUT_KEY_D)) {
        tryingToMove = true;
        sceneData->speed.x += moveAccel * deltatime;
    }
    if (Input_IsKeyPressed(INPUT_KEY_SPACE) && sceneData->fuel > 0.0f) {
        tryingToMove = true;
        if (sceneData->fuel > 0.0f)
            sceneData->speed.y += jumpAccel * deltatime;
    }

    bool jetpacking = tryingToMove && !onGround && sceneData->fuel > 0.0f;

    if (jetpacking) 
        sceneData->fuel -= fuelBurnRate * deltatime;
    else if (sceneData->fuel < c_maxFuel) 
        sceneData->fuel += fuelRefillRate * deltatime;
            
    if (!tryingToMove)
        sceneData->speed.x *= speedDissipationFactor;

    float speedLimitX = jetpacking ? maxJetpackSpeedX : maxSpeedX;

    // Clamp max speed smoothly
    sceneData->speed.x = Lerpf(sceneData->speed.x, Clampf(-speedLimitX, speedLimitX, sceneData->speed.x), 0.2f);
    sceneData->speed.y = Clampf(-maxSpeedY, maxSpeedY, sceneData->speed.y);

    scene->transform.position.x += sceneData->speed.x * deltatime;
    scene->transform.position.y += sceneData->speed.y * deltatime;

    Scene_UpdateGlobalTransform(scene, false);
}

static void Update(Scene* scene, double deltatime)
{
    ASSERT_SCENE_TYPE(scene, SCENE_TYPE_MAIN_CHAR);

    MoveCharacter(scene, deltatime);
    MoveFeet(scene);
    MoveGun(scene);
    Blink(scene, deltatime);
}

static void DrawCharacter(Scene* scene)
{
    MainCharSceneData* sceneData = scene->sceneData;

    Graphics_SetTransformW(&scene->transform);
    Graphics_DrawTextureT(&sceneData->bodyTexture);
    Graphics_ClearTransform();

    // Draw speed
    //Graphics_DrawVectorFromPointW(scene->globalTransform.position, sceneData->speed, COLOR_GREEN);
}

static void DrawUi(Scene* scene)
{
    constexpr float fuelMeterOffsetX = 30.0f;
    constexpr float fuelMeterWidth = 4.0f;
    constexpr float fuelMeterHeight = 30.0f;
    constexpr Color fuelMeterColor = (Color){ 0, 255, 0, 255 };
    constexpr Color fuelMeterBackgroundColor = (Color){ 0, 0, 0, 150 };

    MainCharSceneData* sceneData = (MainCharSceneData*)scene->sceneData;

    if (sceneData->fuel >= 1.0f) return;

    Rect fuelMeterBackgroundRect = {
        .x = scene->globalTransform.position.x - fuelMeterOffsetX,
        .y = scene->globalTransform.position.y + sceneData->bodyTexture.height,
        .width = fuelMeterWidth,
        .height = fuelMeterHeight
    };

    Rect fuelMeterRect = {
        .x = scene->globalTransform.position.x - fuelMeterOffsetX,
        .y = scene->globalTransform.position.y + sceneData->bodyTexture.height,
        .width = fuelMeterWidth,
        .height = fuelMeterHeight * Clampf(0.0f, c_maxFuel, sceneData->fuel)
    };

    Graphics_DrawRectW(fuelMeterBackgroundRect, fuelMeterBackgroundColor);
    Graphics_DrawRectW(fuelMeterRect, fuelMeterColor);
}

static void Draw(Scene* scene)
{
    ASSERT_SCENE_TYPE(scene, SCENE_TYPE_MAIN_CHAR);

    DrawCharacter(scene);
    DrawUi(scene);

    // Draw point at origin
    //Graphics_DrawCircleW(scene->transform.position, 2.0f, COLOR_BLACK);
}

static void OnBodyCollision(Scene* scene, CollisionInfo info)
{
    MainCharSceneData* sceneData = scene->sceneData;

    Vector2 pos = scene->globalTransform.position;

    // Vertical collision
    if (info.collisionRect.width > info.collisionRect.height)
    {
        sceneData->speed.y = 0.0f;

        // Up
        if (info.collisionRect.y > pos.y)
        {
            scene->transform.position.y -= info.collisionRect.height;
        }
        // Down
        else
        {
            scene->transform.position.y += info.collisionRect.height;
        }
    }
    // Horizontal collision
    else
    {
        sceneData->speed.x = 0.0f;

        // Right
        if (info.collisionRect.x > pos.x)
        {
            scene->transform.position.x -= info.collisionRect.width;
        }
        // Left
        else
        {
            scene->transform.position.x += info.collisionRect.width;
        }
    }

    Scene_UpdateGlobalTransform(scene, false);
    ColliderScene_ForceUpdate(sceneData->bodyCollider);
}

static void Cleanup(Scene* scene)
{
    MainCharSceneData* sceneData = scene->sceneData;
    Graphics_UnloadTexture(sceneData->bodyTexture);
}

Scene* MainCharScene_Create(Scene* parent)
{
    Scene* scene = malloc(sizeof(Scene));

    constexpr float legLength = 15.0f;

    MainCharSceneData* sceneData = malloc(sizeof(MainCharSceneData));
    sceneData->speed = Vector2_Zero();
    sceneData->onGround = false;
    sceneData->fuel = c_maxFuel;
    sceneData->elapsedSinceLastBlink = 0.0f;
    sceneData->bodyTexture = Graphics_LoadTexture("res/images/main_char/MainCharBody.png");
    sceneData->bodyToFootMaxDistance = sceneData->bodyTexture.height * 0.5f + legLength;

    scene->sceneData = sceneData;
    scene->type = SCENE_TYPE_MAIN_CHAR;
    scene->transform.position = Vector2_Zero();
    scene->transform.scale = Vector2_New(1.0f, 1.0f);
    scene->transform.origin = Vector2_New(sceneData->bodyTexture.width * 0.5f, (float)sceneData->bodyTexture.height * 0.5f);
    scene->transform.rotation = 0.0f;
    scene->transform.topLevel = false;
    scene->childrenCount = 0u;
    scene->name = "Main Char Scene";
    scene->parent = parent;

    scene->startFunction = NULL;
    scene->updateFunction = Update;
    scene->drawFunction = Draw;
    scene->cleanupFunction = Cleanup;

    Scene_UpdateGlobalTransform(scene, false);

    Scene* leftFoot = MainCharFootScene_Create(scene);
    Scene* rightFoot = MainCharFootScene_Create(scene);
    leftFoot->transform.topLevel = true;
    rightFoot->transform.topLevel = true;
    sceneData->leftFoot = leftFoot;
    sceneData->rightFoot = rightFoot;
    Scene_AddChild(scene, leftFoot);
    Scene_AddChild(scene, rightFoot);

    // constexpr float handsDistance = 20.0f;
    // Scene* leftHand = MainCharHandScene_Create(scene);
    // Scene* rightHand = MainCharHandScene_Create(scene);
    // leftHand->transform.position.x -= handsDistance;
    // rightHand->transform.position.x += handsDistance;
    // sceneData->leftHand = leftHand;
    // sceneData->rightHand = rightHand;
    // Scene_AddChild(scene, leftHand);
    // Scene_AddChild(scene, rightHand);


    constexpr float c_eyeOffsetX = 5.0f;
    constexpr float c_eyeOffsetY = 10.0f;
    Scene* leftEye = MainCharEyeScene_Create(scene);
    Scene* rightEye = MainCharEyeScene_Create(scene);
    leftEye->transform.position.y += sceneData->bodyTexture.height * 0.5f + c_eyeOffsetY;
    leftEye->transform.position.x -= c_eyeOffsetX;
    rightEye->transform.position.y += sceneData->bodyTexture.height * 0.5f + c_eyeOffsetY;
    rightEye->transform.position.x += c_eyeOffsetX;
    sceneData->leftEye = leftEye;
    sceneData->rightEye = rightEye;
    Scene_AddChild(scene, leftEye);
    Scene_AddChild(scene, rightEye);

    Scene* gun = MainCharGunScene_Create(scene);
    sceneData->gun = gun;
    sceneData->gun->transform.topLevel = true;
    Scene_AddChild(scene, gun);

    Vector2 colliderSize = {
        .x = scene->transform.origin.x * 2.0f,
        .y = scene->transform.origin.y * 2.0f * 2.0f,
    };
    Vector2 colliderPosition = {
        .x = -colliderSize.x * 0.5f,
        .y = -colliderSize.y * 0.5f
    };

    Scene* colliderScene = ColliderScene_Create(scene, colliderSize, "Main Char Scene Body Collider");
    sceneData->bodyCollider = colliderScene;
    colliderScene->transform.position = colliderPosition;
    ColliderScene_SetVisible(colliderScene, false);
    ColliderScene_SetOnCollisionCallback(colliderScene, scene, OnBodyCollision);
    ColliderScene_SetCollisionLayers(colliderScene, COLLIDER_LAYER_MAIN_CHAR);
    ColliderScene_SetCollisionScan(colliderScene, COLLIDER_LAYER_WORLD);
    Scene_AddChild(scene, colliderScene);

    Scene* onGroundRaycastScene = RaycastScene_Create(scene, Vector2_Down(), legLength + sceneData->bodyTexture.height * 0.5f + 1.0f);
    RaycastScene_SetVisible(onGroundRaycastScene, false);
    sceneData->onGroundRaycast = onGroundRaycastScene;
    Scene_AddChild(scene, onGroundRaycastScene);

    return scene;
}