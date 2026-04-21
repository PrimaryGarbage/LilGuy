#include "raycast_scene.h"
#include "graphics/color.h"
#include "graphics/graphics.h"
#include "physics/raycast.h"
#include "physics/transform.h"
#include "scene.h"
#include "scene/scene.h"
#include "scene_type.h"
#include "vector2.h"
#include <stdlib.h>

typedef struct RaycastSceneData {
    RaycastScene_OnCollisionCallback onCollisionCallback;
    Scene* onCollisioonCallbackOwner;
    float length;
    bool visible;
} RaycastSceneData;

void Update(Scene* scene, double _)
{
    RaycastSceneData* sceneData = scene->sceneData;

    if (sceneData->onCollisionCallback)
    {
        Raycast raycast = Raycast_New(scene->globalTransform.position, Transform_Forward(&scene->globalTransform), sceneData->length);
        Vector2 collisionPoint;
        if (Raycast_CheckForCollision(&raycast, &collisionPoint))
            sceneData->onCollisionCallback(sceneData->onCollisioonCallbackOwner, collisionPoint);
    }
}

void Draw(Scene* scene)
{
    RaycastSceneData* sceneData = scene->sceneData;

    if (!sceneData->visible) return;

    Graphics_DrawVectorFromPoint(scene->globalTransform.position, Vector2_MultScalar(Transform_Forward(&scene->globalTransform), sceneData->length), COLOR_GREEN);
}

Scene* RaycastScene_Create(Scene* parent, Vector2 direction, float length, const char* name)
{
    Scene* scene = malloc(sizeof(Scene));
    Scene_DefaultInit(scene, SCENE_TYPE_RAYCAST, parent, name);
    RaycastSceneData* sceneData = malloc(sizeof(RaycastSceneData));
    sceneData->length = length;
    sceneData->onCollisionCallback = NULL;
    sceneData->visible = false;
    scene->sceneData = sceneData;

    scene->updateFunction = Update;
    scene->drawFunction = Draw;

    scene->transform.rotation = Vector2_Angle(direction);

    return scene;
}

void RaycastScene_SetOnCollisionCallback(Scene* scene, Scene* callbackOwner, RaycastScene_OnCollisionCallback callback)
{
    ASSERT_SCENE_TYPE(scene, SCENE_TYPE_RAYCAST);

    RaycastSceneData* sceneData = scene->sceneData;
    sceneData->onCollisionCallback = callback;
    sceneData->onCollisioonCallbackOwner = callbackOwner;
}

bool RaycastScene_CheckForCollision(Scene* scene, Vector2* collisionPoint_out)
{
    ASSERT_SCENE_TYPE(scene, SCENE_TYPE_RAYCAST);

    RaycastSceneData* sceneData = scene->sceneData;

    Scene_UpdateGlobalTransform(scene);

    Raycast raycast = Raycast_New(scene->globalTransform.position, Transform_Forward(&scene->globalTransform), sceneData->length);

    Vector2 point;
    if (Raycast_CheckForCollision(&raycast, &point))
    {
        if (collisionPoint_out) *collisionPoint_out = point;
        return true;
    }

    return false;
}

void RaycastScene_SetVisible(Scene* scene, bool on)
{
    ASSERT_SCENE_TYPE(scene, SCENE_TYPE_RAYCAST);

    ((RaycastSceneData*)scene->sceneData)->visible = on;
}

void RaycastScene_SetLength(Scene* scene, float length)
{
    ASSERT_SCENE_TYPE(scene, SCENE_TYPE_RAYCAST);

    ((RaycastSceneData*)scene->sceneData)->length = length;
}

float RaycastScene_GetLength(Scene* scene)
{
    ASSERT_SCENE_TYPE(scene, SCENE_TYPE_RAYCAST);

    return ((RaycastSceneData*)scene->sceneData)->length;
}

Vector2 RaycastScene_GetRaycastVector(Scene* scene)
{
    ASSERT_SCENE_TYPE(scene, SCENE_TYPE_RAYCAST);
    RaycastSceneData* sceneData = scene->sceneData;

    return Vector2_MultScalar(Transform_Forward(&scene->globalTransform), sceneData->length);
}

Vector2 RaycastScene_GetGlobalRaycastPointToVector(Scene* scene)
{
    ASSERT_SCENE_TYPE(scene, SCENE_TYPE_RAYCAST);
    RaycastSceneData* sceneData = scene->sceneData;

    return Vector2_Add(scene->globalTransform.position, Vector2_MultScalar(Transform_Forward(&scene->globalTransform), sceneData->length));
}