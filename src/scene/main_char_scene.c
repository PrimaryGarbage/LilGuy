#include "main_char_scene.h"
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
    Scene* leftFoot;
    Scene* rightFoot;
    bool movingLeftFoot;
    bool movingRightFoot;
} MainCharSceneData;

static const float c_bodyHeight = 30.0f;
static const float c_bodyWidth = 10.0f;
static const float c_legLength = 10.0f;
static const float c_footIdleTargetOffsetX = 10.0f;
static const float c_footGroundTargetOffsetY = c_legLength + c_bodyHeight * 0.5f;
static const float c_footLiftedTargetOffsetY = c_legLength + c_bodyHeight * 0.3f;
static const double c_footMoveAnimationLength = 0.05;
static const float c_footMaxDistanceFromOriginX = 30.0f;
static const float c_footStrideX = 28.0f;

static Vector2 GetNewFootPosition(Scene* scene)
{
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
    MainCharSceneData* sceneData = (MainCharSceneData*)scene->sceneData;

    if (sceneData->onGround)
    {
        // Check if at least one foot is in valid distance
        // if not, move the foot with bigger distance
        float leftFootDistance = fabsf(sceneData->leftFoot->transform.position.x - scene->globalTransform.position.x);
        float rightFootDistance = fabsf(sceneData->rightFoot->transform.position.x - scene->globalTransform.position.x);

        if (leftFootDistance > c_footMaxDistanceFromOriginX || rightFootDistance > c_footMaxDistanceFromOriginX)
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
    MainCharSceneData* customData = (MainCharSceneData*)scene->sceneData;

    const float moveAccel = 1000.0f;
    const float jumpAccel = 1500.0f;
    const float gravityAccel = 1000.0f;
    const float maxGroundSpeedX = 200.0f;
    const float maxAirSpeedX = 500.0f;
    const float maxSpeedY = 2000.0f;
    const float speedDissipationFactor = 0.8f;

    customData->speed.y += gravityAccel * deltatime;

    bool tryingToMove = false;

    if (Input_IsKeyPressed(INPUT_KEY_A)) {
        customData->speed.x -= moveAccel * deltatime;
        tryingToMove = true;
    }
    if (Input_IsKeyPressed(INPUT_KEY_D)) {
        customData->speed.x += moveAccel * deltatime;
        tryingToMove = true;
    }
    if (Input_IsKeyPressed(INPUT_KEY_SPACE)) {
        customData->speed.y -= jumpAccel * deltatime;
        customData->onGround = false;
        tryingToMove = true;
    }

    if (!tryingToMove)
        customData->speed.x *= speedDissipationFactor;

    ClampOnScreenPosition(&scene->transform, customData);

    float speedLimitX = customData->onGround ? maxGroundSpeedX : maxAirSpeedX;

    customData->speed.x = Clampf(-speedLimitX, speedLimitX, customData->speed.x);
    customData->speed.y = Clampf(-maxSpeedY, maxSpeedY, customData->speed.y);

    scene->transform.position.x += customData->speed.x * deltatime;
    scene->transform.position.y += customData->speed.y * deltatime;

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

static void DrawCallback(Scene* scene)
{
    ASSERT_SCENE_TYPE(scene, SCENE_TYPE_MAIN_CHAR);

    DrawCharacter(scene);

    // Draw point at origin
    Graphics_DrawRect(Rect_FromVectors(scene->transform.position, Vector2_Uniform(2.0f)), COLOR_WHITE);
}

Scene* MainCharScene_Create(Scene* parent)
{
    Scene* scene = malloc(sizeof(Scene));

    MainCharSceneData* customData = malloc(sizeof(MainCharSceneData));
    customData->speed = Vector2_Zero();
    customData->onGround = false;

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
    leftFoot->transform.position.x = scene->globalTransform.position.x - c_footIdleTargetOffsetX;
    leftFoot->transform.position.y = scene->globalTransform.position.y + c_legLength + c_bodyHeight * 0.5f;
    rightFoot->transform.position.x = scene->globalTransform.position.x + c_footIdleTargetOffsetX;
    rightFoot->transform.position.y = scene->globalTransform.position.y + c_legLength + c_bodyHeight * 0.5f;

    customData->leftFoot = leftFoot;
    customData->rightFoot = rightFoot;

    Scene_AddChild(scene, leftFoot);
    Scene_AddChild(scene, rightFoot);

    scene->startFunction = NULL;
    scene->updateFunction = UpdateCallback;
    scene->drawFunction = DrawCallback;

    return scene;
}
