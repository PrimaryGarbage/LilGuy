#include "main_char_scene.h"
#include "graphics/color.h"
#include "input/input.h"
#include "main_char_foot_scene.h"
#include "math_helpers.h"
#include "scene.h"
#include "scene_type.h"
#include "transform.h"
#include "vector2.h"
#include "graphics/graphics.h"
#include <math.h>

typedef struct MainCharSceneData {
    Vector2 speed;
    bool onGround;
    float fuel;
    Scene* leftFoot;
    Scene* rightFoot;
    bool movingLeftFoot;
    bool movingRightFoot;
} MainCharSceneData;

constexpr float c_maxFuel = 1.0f;
constexpr float c_bodyHeight = 30.0f;
constexpr float c_bodyWidth = 10.0f;
constexpr float c_legLength = 10.0f;
constexpr float c_footIdleTargetOffsetX = 10.0f;
constexpr float c_footGroundTargetOffsetY = c_legLength + c_bodyHeight * 0.5f;

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

static void ClampOnScreenPosition(Transform* transform, MainCharSceneData* customData)
{
    Rect screenRect = Graphics_GetScreenRect();

    if (transform->position.x < screenRect.x + transform->origin.x)
    {
        transform->position.x = screenRect.x + transform->origin.x;
        customData->speed.x = 0.0f;
        // friction
        // customData->speed.y *= 0.5f;
    }
    else if (transform->position.x > screenRect.width - transform->origin.x)
    {
        transform->position.x = screenRect.width - transform->origin.x;
        customData->speed.x = 0.0f;
        // friction
        // customData->speed.y *= 0.5f;
    }

    if (transform->position.y < screenRect.y + transform->origin.y)
    {
        transform->position.y = screenRect.y + transform->origin.y;
        customData->speed.y = 0.0f;

        // friction
        // customData->speed.x *= 0.5f;
    }
    else if (transform->position.y > screenRect.height - transform->origin.y)
    {
        transform->position.y = screenRect.height - transform->origin.y;
        customData->speed.y = 0.0f;
        customData->onGround = true;
        // friction
        // customData->speed.x *= 0.5f;
    }
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

static void MoveCharacter(Scene* scene, double deltatime)
{
    MainCharSceneData* sceneData = (MainCharSceneData*)scene->sceneData;

    constexpr float moveAccel = 1000.0f;
    constexpr float jumpAccel = 1500.0f;
    constexpr float gravityAccel = 1000.0f;
    constexpr float maxGroundSpeedX = 200.0f;
    constexpr float maxAirSpeedX = 500.0f;
    constexpr float maxSpeedY = 2000.0f;
    constexpr float speedDissipationFactor = 0.8f;
    constexpr float fuelBurnRate = 0.5f;
    constexpr float fuelRefillRate = 0.3f;

    sceneData->speed.y += gravityAccel * deltatime;

    bool tryingToMove = false;

    if (Input_IsKeyPressed(INPUT_KEY_A)) {
        sceneData->speed.x -= moveAccel * deltatime;
        tryingToMove = true;
    }
    if (Input_IsKeyPressed(INPUT_KEY_D)) {
        sceneData->speed.x += moveAccel * deltatime;
        tryingToMove = true;
    }
    if (Input_IsKeyPressed(INPUT_KEY_SPACE) && sceneData->fuel > 0.0f) {
        sceneData->fuel -= fuelBurnRate * deltatime;
        sceneData->speed.y -= jumpAccel * deltatime;
        sceneData->onGround = false;
    }
    else if (sceneData->fuel < c_maxFuel)
    {
        sceneData->fuel += fuelRefillRate * deltatime;
    }

    if (!tryingToMove)
        sceneData->speed.x *= speedDissipationFactor;

    ClampOnScreenPosition(&scene->transform, sceneData);

    float speedLimitX = sceneData->onGround ? maxGroundSpeedX : maxAirSpeedX;

    sceneData->speed.x = Clampf(-speedLimitX, speedLimitX, sceneData->speed.x);
    sceneData->speed.y = Clampf(-maxSpeedY, maxSpeedY, sceneData->speed.y);

    scene->transform.position.x += sceneData->speed.x * deltatime;
    scene->transform.position.y += sceneData->speed.y * deltatime;

    Scene_UpdateGlobalTransform(scene, false);
}

static void UpdateCallback(Scene* scene, double deltatime)
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

static void DrawCallback(Scene* scene)
{
    ASSERT_SCENE_TYPE(scene, SCENE_TYPE_MAIN_CHAR);

    DrawCharacter(scene);
    DrawUi(scene);

    // Draw point at origin
    Graphics_DrawRect(Rect_FromVectors(scene->transform.position, Vector2_Uniform(2.0f)), COLOR_WHITE);
}

Scene* MainCharScene_Create(Scene* parent)
{
    Scene* scene = malloc(sizeof(Scene));

    MainCharSceneData* customData = malloc(sizeof(MainCharSceneData));
    customData->speed = Vector2_Zero();
    customData->onGround = false;
    customData->fuel = c_maxFuel;

    scene->sceneData = customData;
    scene->type = SCENE_TYPE_MAIN_CHAR;
    scene->transform.position = Vector2_New(Graphics_GetScreenWidth() * 0.5f, Graphics_GetScreenHeight() * 0.5f);
    scene->transform.scale = Vector2_New(1.0f, 1.0f);
    scene->transform.origin = Vector2_New(c_bodyWidth * 0.5f, c_bodyHeight * 0.5f + c_legLength);
    scene->transform.rotation = 0.0f;
    scene->transform.topLevel = false;
    scene->childrenCount = 0u;
    scene->parent = parent;

    Scene_UpdateGlobalTransform(scene, false);

    Scene* leftFoot = MainCharFootScene_Create(scene, GetNewFootPosition);
    Scene* rightFoot = MainCharFootScene_Create(scene, GetNewFootPosition);
    leftFoot->transform.topLevel = true;
    rightFoot->transform.topLevel = true;

    customData->leftFoot = leftFoot;
    customData->rightFoot = rightFoot;

    Scene_AddChild(scene, leftFoot);
    Scene_AddChild(scene, rightFoot);

    scene->startFunction = NULL;
    scene->updateFunction = UpdateCallback;
    scene->drawFunction = DrawCallback;

    return scene;
}
