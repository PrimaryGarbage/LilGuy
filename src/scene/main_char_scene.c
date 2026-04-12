#include "main_char_scene.h"
#include "logging.h"
#include "physics/collider.h"
#include "collider_scene.h"
#include "graphics/color.h"
#include "input/input.h"
#include "main_char_foot_scene.h"
#include "math_helpers.h"
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
    Scene* leftFoot;
    Scene* rightFoot;
    Scene* bodyCollider;
    Scene* onGroundRaycast;
    bool movingLeftFoot;
    bool movingRightFoot;
} MainCharSceneData;

constexpr float c_maxFuel = 1.0f;
constexpr float c_bodyHeight = 30.0f;
constexpr float c_bodyWidth = 10.0f;
constexpr float c_legLength = 10.0f;
constexpr float c_footIdleTargetOffsetX = 10.0f;
constexpr float c_footGroundTargetOffsetY = c_legLength + c_bodyHeight * 0.5f;
constexpr double c_landingAnimationLength = 0.2f;

static Vector2 GetNewFootPosition(Scene* scene)
{
    constexpr float c_footStrideX = 28.0f;

    ASSERT_SCENE_TYPE(scene, SCENE_TYPE_MAIN_CHAR);

    MainCharSceneData* sceneData = (MainCharSceneData*)scene->sceneData;

    Vector2 newPos = scene->globalTransform.position;

    if (sceneData->onGround)
    {
        newPos.x += Signf(sceneData->speed.x) * c_footStrideX;
        newPos.y += c_footGroundTargetOffsetY;
    }
    else
    {
        newPos.x += c_footIdleTargetOffsetX;
        newPos.y += c_footGroundTargetOffsetY;
    }

    return newPos;
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
        sceneData->leftFoot->transform.position.x = Lerpf(sceneData->leftFoot->transform.position.x, scene->globalTransform.position.x - c_footIdleTargetOffsetX, lerpWeight);
        sceneData->leftFoot->transform.position.y = Lerpf(sceneData->leftFoot->transform.position.y, scene->globalTransform.position.y + c_footGroundTargetOffsetY, lerpWeight);
        sceneData->rightFoot->transform.position.x = Lerpf(sceneData->rightFoot->transform.position.x, scene->globalTransform.position.x + c_footIdleTargetOffsetX, lerpWeight);
        sceneData->rightFoot->transform.position.y = Lerpf(sceneData->rightFoot->transform.position.y, scene->globalTransform.position.y + c_footGroundTargetOffsetY, lerpWeight);
    }
}

static void LandingTweenAnimationCallback(Scene* scene, double elapsed)
{
    constexpr float animationMaxOffsetY = 10.0f;

    float weight = elapsed / c_landingAnimationLength;
    float offsetWeight = weight < 0.5f ? weight * 2.0f : 2.0f - weight * 2.0f;
    scene->transform.position.y += animationMaxOffsetY * offsetWeight;
}

static void OnLanding(Scene* scene)
{
    MainCharSceneData* sceneData = (MainCharSceneData*)scene->sceneData;

    sceneData->leftFoot->transform.position.x = scene->globalTransform.position.x - c_footIdleTargetOffsetX;
    sceneData->leftFoot->transform.position.y = scene->globalTransform.position.y + c_footGroundTargetOffsetY;
    sceneData->rightFoot->transform.position.x = scene->globalTransform.position.x + c_footIdleTargetOffsetX;
    sceneData->rightFoot->transform.position.y = scene->globalTransform.position.y + c_footGroundTargetOffsetY;

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
    constexpr float fuelBurnRate = 0.5f;
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
            sceneData->speed.y -= jumpAccel * deltatime;
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
}

static void DrawCharacter(Scene* scene)
{
    Graphics_SetTransform(&scene->transform);
    Graphics_DrawRectT(Vector2_New(c_bodyWidth, c_bodyHeight), COLOR_PURPLE);
    Graphics_ClearTransform();
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
        .y = scene->globalTransform.position.y - c_bodyHeight,
        .width = fuelMeterWidth,
        .height = fuelMeterHeight
    };

    Rect fuelMeterRect = {
        .x = scene->globalTransform.position.x - fuelMeterOffsetX,
        .y = scene->globalTransform.position.y - c_bodyHeight,
        .width = fuelMeterWidth,
        .height = fuelMeterHeight * Clampf(0.0f, c_maxFuel, sceneData->fuel)
    };

    Graphics_DrawRect(fuelMeterBackgroundRect, fuelMeterBackgroundColor);
    Graphics_DrawRect(fuelMeterRect, fuelMeterColor);
}

static void Draw(Scene* scene)
{
    ASSERT_SCENE_TYPE(scene, SCENE_TYPE_MAIN_CHAR);

    DrawCharacter(scene);
    DrawUi(scene);

    // Draw point at origin
    Graphics_DrawRect(Rect_FromVectors(scene->transform.position, Vector2_Uniform(2.0f)), COLOR_WHITE);
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
            //Graphics_DrawVector(Vector2_Add(pos,(Vector2){ 0.0f, 50.0f }), pos, COLOR_GREEN);
        }
        // Up
        else
        {
            scene->transform.position.y += info.collisionRect.height;
            //Graphics_DrawVector(Vector2_Add(pos,(Vector2){ 0.0f, -50.0f }), pos, COLOR_GREEN);
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
            //Graphics_DrawVector(Vector2_Add(pos,(Vector2){ 50.0f, 0.0f }), pos, COLOR_GREEN);
        }
        // Left
        else
        {
            scene->transform.position.x += info.collisionRect.width;
            //Graphics_DrawVector(Vector2_Add(pos,(Vector2){ -50.0f, 0.0f }), pos, COLOR_GREEN);
        }
    }

    Scene_UpdateGlobalTransform(scene, false);
    ColliderScene_ForceUpdate(sceneData->bodyCollider);
}

Scene* MainCharScene_Create(Scene* parent)
{
    Scene* scene = malloc(sizeof(Scene));

    MainCharSceneData* sceneData = malloc(sizeof(MainCharSceneData));
    sceneData->speed = Vector2_Zero();
    sceneData->onGround = false;
    sceneData->fuel = c_maxFuel;

    scene->sceneData = sceneData;
    scene->type = SCENE_TYPE_MAIN_CHAR;
    scene->transform.position = Vector2_Zero();
    scene->transform.scale = Vector2_New(1.0f, 1.0f);
    scene->transform.origin = Vector2_New(c_bodyWidth * 0.5f, c_bodyHeight * 0.5f + c_legLength);
    scene->transform.rotation = 0.0f;
    scene->transform.topLevel = false;
    scene->childrenCount = 0u;
    scene->parent = parent;

    scene->startFunction = NULL;
    scene->updateFunction = Update;
    scene->drawFunction = Draw;
    scene->cleanupFunction = NULL;

    Scene_UpdateGlobalTransform(scene, false);

    Scene* leftFoot = MainCharFootScene_Create(scene, GetNewFootPosition);
    Scene* rightFoot = MainCharFootScene_Create(scene, GetNewFootPosition);
    leftFoot->transform.topLevel = true;
    rightFoot->transform.topLevel = true;

    sceneData->leftFoot = leftFoot;
    sceneData->rightFoot = rightFoot;

    Scene_AddChild(scene, leftFoot);
    Scene_AddChild(scene, rightFoot);

    Vector2 colliderPosition = {
        .x = -scene->transform.origin.x - 10.0f,
        .y = -scene->transform.origin.y,
    };
    Vector2 colliderSize = {
        .x = scene->transform.origin.x * 2.0f + 20.0f,
        .y = scene->transform.origin.y * 2.0f,
    };

    Scene* colliderScene = ColliderScene_Create(scene, colliderSize);
    sceneData->bodyCollider = colliderScene;
    colliderScene->transform.position = colliderPosition;
    ColliderScene_SetVisible(colliderScene, false);
    ColliderScene_SetOnCollisionCallback(colliderScene, scene, OnBodyCollision);
    ColliderScene_SetCollisionLayers(colliderScene, COLLIDER_LAYER_MAIN_CHAR);
    ColliderScene_SetCollisionScan(colliderScene, COLLIDER_LAYER_WORLD);
    Scene_AddChild(scene, colliderScene);

    Scene* onGroundRaycastScene = RaycastScene_Create(scene, Vector2_Down(), c_legLength + c_bodyHeight * 0.5f + 1.0f);
    sceneData->onGroundRaycast = onGroundRaycastScene;
    Scene_AddChild(scene, onGroundRaycastScene);


    return scene;
}
