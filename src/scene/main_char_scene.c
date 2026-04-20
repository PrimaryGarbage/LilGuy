#include "main_char_scene.h"
#include "graphics/draw_order.h"
#include "input/input_button.h"
#include "logging.h"
#include "main_char_gun_scene.h"
#include "main_char_eye_scene.h"
#include "main_char_jetpack_fire_scene.h"
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
    Scene* jetpackFire;
    float elapsedSinceLastBlink;
    float bodyToFootMaxDistance;
    bool movingLeftFoot;
    bool movingRightFoot;
    bool usingJetpack;
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
        newPos.y += sceneData->bodyToFootMaxDistance;
    }
    else
    {
        newPos.x += c_idleDistanceBetweenFeet;
        newPos.y += sceneData->bodyToFootMaxDistance;
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
    constexpr Vector2 positionOffset = (Vector2) { .x = 0.0f, .y = 10.0f };

    MainCharSceneData* sceneData = (MainCharSceneData*)scene->sceneData;

    Vector2 mousePos = Graphics_GetMousePosition();
    Vector2 dir = Vector2_Sub(mousePos, sceneData->gun->transform.position);
    float angle = Vector2_Angle(dir);

    constexpr float lerpWeight = 0.4f;
    sceneData->gun->transform.position = Vector2_Lerp(sceneData->gun->transform.position, Vector2_Add(scene->globalTransform.position, positionOffset), lerpWeight);
    sceneData->gun->transform.rotation = LerpAnglef(sceneData->gun->transform.rotation, angle, lerpWeight);
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
        constexpr float lerpWeight = 0.6f;
        sceneData->leftFoot->transform.position.x = Lerpf(sceneData->leftFoot->transform.position.x, scene->globalTransform.position.x - c_idleDistanceBetweenFeet, lerpWeight);
        sceneData->leftFoot->transform.position.y = Lerpf(sceneData->leftFoot->transform.position.y, scene->globalTransform.position.y + sceneData->bodyToFootMaxDistance, lerpWeight);
        sceneData->rightFoot->transform.position.x = Lerpf(sceneData->rightFoot->transform.position.x, scene->globalTransform.position.x + c_idleDistanceBetweenFeet, lerpWeight);
        sceneData->rightFoot->transform.position.y = Lerpf(sceneData->rightFoot->transform.position.y, scene->globalTransform.position.y + sceneData->bodyToFootMaxDistance, lerpWeight);
    }
}

static void LandingTweenAnimationCallback(Scene* scene, double weight, double _)
{
    constexpr float animationMaxOffsetY = 10.0f;

    float offsetWeight = weight < 0.5f ? weight * 2.0f : 2.0f - weight * 2.0f;
    scene->transform.position.y += animationMaxOffsetY * offsetWeight;
}

static void OnLanding(Scene* scene)
{
    MainCharSceneData* sceneData = (MainCharSceneData*)scene->sceneData;

    sceneData->leftFoot->transform.position.x = scene->globalTransform.position.x - c_idleDistanceBetweenFeet;
    sceneData->leftFoot->transform.position.y = scene->globalTransform.position.y + sceneData->bodyToFootMaxDistance;
    sceneData->rightFoot->transform.position.x = scene->globalTransform.position.x + c_idleDistanceBetweenFeet;
    sceneData->rightFoot->transform.position.y = scene->globalTransform.position.y + sceneData->bodyToFootMaxDistance;

    Tween_CreateFunction(c_landingAnimationLength, scene, LandingTweenAnimationCallback, TWEEN_INTERPOLATION_QUADRATIC);
}

static void MoveCharacter(Scene* scene, double deltatime)
{
    MainCharSceneData* sceneData = (MainCharSceneData*)scene->sceneData;

    constexpr float moveAccel = 1000.0f;
    constexpr float jumpAccel = -1500.0f;
    constexpr float gravityAccel = 1000.0f;
    constexpr float maxSpeedX = 180.0f;
    constexpr float maxJetpackSpeedX = 400.0f;
    constexpr float maxSpeedY = 1000.0f;
    constexpr float speedDissipationFactor = 0.8f;
    constexpr float fuelBurnRate = 0.4f;
    constexpr float fuelRefillRate = 0.3f;

    sceneData->speed.y += gravityAccel * deltatime;

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

    sceneData->usingJetpack = tryingToMove && !onGround && sceneData->fuel > 0.0f;

    if (sceneData->usingJetpack)
    {
        MainCharJetpackFireScene_Show(sceneData->jetpackFire);
        float angle = (sceneData->speed.x / maxJetpackSpeedX) * 45.0f;
        sceneData->jetpackFire->transform.rotation = LerpAnglef(sceneData->jetpackFire->transform.rotation, angle, 0.3f);
    }
    else
    {
        MainCharJetpackFireScene_Hide(sceneData->jetpackFire);
    }


    if (sceneData->usingJetpack) 
        sceneData->fuel -= fuelBurnRate * deltatime;
    else if (sceneData->fuel < c_maxFuel) 
        sceneData->fuel += fuelRefillRate * deltatime;
            
    if (!tryingToMove)
        sceneData->speed.x *= speedDissipationFactor;

    float speedLimitX = sceneData->usingJetpack ? maxJetpackSpeedX : maxSpeedX;

    // Clamp max speed smoothly
    sceneData->speed.x = Lerpf(sceneData->speed.x, Clampf(-speedLimitX, speedLimitX, sceneData->speed.x), 0.2f);
    sceneData->speed.y = Clampf(-maxSpeedY, maxSpeedY, sceneData->speed.y);

    scene->transform.position.x += sceneData->speed.x * deltatime;
    scene->transform.position.y += sceneData->speed.y * deltatime;

    Scene_UpdateGlobalTransform(scene);
}

static void Shoot(Scene* scene)
{
    if (Input_IsMouseButtonPressed(INPUT_MOUSE_BUTTON_LEFT))
    {
        MainCharSceneData* sceneData = (MainCharSceneData*)scene->sceneData;
        MainCharGunScene_Shoot(sceneData->gun);
    }
}

static void Update(Scene* scene, double deltatime)
{
    ASSERT_SCENE_TYPE(scene, SCENE_TYPE_MAIN_CHAR);

    MoveCharacter(scene, deltatime);
    MoveFeet(scene);
    MoveGun(scene);
    Shoot(scene);
    Blink(scene, deltatime);
}

static void DrawCharacter(Scene* scene)
{
    MainCharSceneData* sceneData = scene->sceneData;

    Graphics_SetModelMatrix(&scene->transform);
    Graphics_DrawTextureT(&sceneData->bodyTexture, DRAW_ORDER_MAIN_CHAR, COLOR_WHITE);
    Graphics_ClearModelMatrix();

    // Draw speed
    //Graphics_DrawVectorFromPointW(scene->globalTransform.position, sceneData->speed, COLOR_GREEN);
}

static void DrawUi(Scene* scene)
{
    constexpr float fuelMeterOffsetX = 30.0f;
    constexpr float fuelMeterWidth = 4.0f;
    constexpr float fuelMeterHeight = 30.0f;
    constexpr Color fuelMeterFullColor = (Color){ 0, 255, 0, 255 };
    constexpr Color fuelMeterEmptyColor = (Color){ 255, 0, 0, 255 };
    constexpr Color fuelMeterBackgroundColor = (Color){ 0, 0, 0, 150 };

    MainCharSceneData* sceneData = (MainCharSceneData*)scene->sceneData;

    if (sceneData->fuel >= 1.0f) return;

    Rect fuelMeterBackgroundRect = {
        .x = scene->globalTransform.position.x - fuelMeterOffsetX,
        .y = scene->globalTransform.position.y - sceneData->bodyTexture.height,
        .width = fuelMeterWidth,
        .height = fuelMeterHeight
    };

    float fuelHeight = fuelMeterHeight * Clampf(0.0f, c_maxFuel, sceneData->fuel);

    Rect fuelMeterRect = {
        .x = scene->globalTransform.position.x - fuelMeterOffsetX,
        .y = scene->globalTransform.position.y - fuelHeight,
        .width = fuelMeterWidth,
        .height = fuelHeight
    };

    Graphics_DrawRect(fuelMeterBackgroundRect, fuelMeterBackgroundColor, DRAW_ORDER_UI);
    Graphics_DrawRect(fuelMeterRect, Color_Lerp(&fuelMeterEmptyColor, &fuelMeterFullColor, sceneData->fuel / c_maxFuel), DRAW_ORDER_UI);
}

static void Draw(Scene* scene)
{
    ASSERT_SCENE_TYPE(scene, SCENE_TYPE_MAIN_CHAR);

    DrawCharacter(scene);
    DrawUi(scene);

    // Draw point at origin
    //Graphics_DrawCircle(scene->globalTransform.position, 2.0f, COLOR_RED, DRAW_ORDER_TOP);
}

static void OnBodyCollision(Scene* scene, CollisionInfo info)
{
    MainCharSceneData* sceneData = scene->sceneData;

    Vector2 pos = scene->globalTransform.position;

    // Vertical collision
    if (info.collisionRect.width > info.collisionRect.height)
    {
        sceneData->speed.y = 0.0f;

        // Down
        if (info.collisionRect.y > pos.y)
        {
            scene->transform.position.y -= info.collisionRect.height;
        }
        // Up
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

    Scene_UpdateGlobalTransform(scene);
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
    Scene_DefaultInit(scene, SCENE_TYPE_MAIN_CHAR, "Main Char");
    if (parent) Scene_AddChild(parent, scene);

    constexpr float legLength = 15.0f;

    MainCharSceneData* sceneData = malloc(sizeof(MainCharSceneData));
    sceneData->speed = Vector2_Zero();
    sceneData->onGround = false;
    sceneData->fuel = c_maxFuel;
    sceneData->elapsedSinceLastBlink = 0.0f;
    sceneData->bodyTexture = Graphics_LoadTexture("res/images/main_char/MainCharBody.png");
    sceneData->bodyToFootMaxDistance = sceneData->bodyTexture.height * 0.5f + legLength;

    scene->sceneData = sceneData;
    scene->transform.origin = Vector2_New(sceneData->bodyTexture.width * 0.5f, (float)sceneData->bodyTexture.height * 0.5f);

    scene->updateFunction = Update;
    scene->drawFunction = Draw;
    scene->cleanupFunction = Cleanup;

    Scene_UpdateGlobalTransform(scene);

    Scene* leftFoot = MainCharFootScene_Create(scene);
    Scene* rightFoot = MainCharFootScene_Create(scene);
    leftFoot->transform.topLevel = true;
    rightFoot->transform.topLevel = true;
    sceneData->leftFoot = leftFoot;
    sceneData->rightFoot = rightFoot;

    constexpr float c_eyeOffsetX = 5.0f;
    constexpr float c_eyeOffsetY = 10.0f;
    Scene* leftEye = MainCharEyeScene_Create(scene);
    Scene* rightEye = MainCharEyeScene_Create(scene);
    leftEye->transform.position.y -= sceneData->bodyTexture.height * 0.5f + c_eyeOffsetY;
    leftEye->transform.position.x -= c_eyeOffsetX;
    rightEye->transform.position.y -= sceneData->bodyTexture.height * 0.5f + c_eyeOffsetY;
    rightEye->transform.position.x += c_eyeOffsetX;
    sceneData->leftEye = leftEye;
    sceneData->rightEye = rightEye;

    Scene* gun = MainCharGunScene_Create(scene);
    gun->transform.topLevel = true;
    sceneData->gun = gun;

    Scene* jetpackFire = MainCharJetpackFireScene_Create(scene);
    sceneData->jetpackFire = jetpackFire;
    jetpackFire->transform.position.y += (float)sceneData->bodyTexture.height * 0.5f - 10.0f;

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

    Scene* onGroundRaycastScene = RaycastScene_Create(scene, Vector2_Down(), legLength + sceneData->bodyTexture.height * 0.5f + 1.0f, "OnGround Raycast");
    RaycastScene_SetVisible(onGroundRaycastScene, false);
    sceneData->onGroundRaycast = onGroundRaycastScene;

    return scene;
}