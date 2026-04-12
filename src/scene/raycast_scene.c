#include "raycast_scene.h"
#include "physics/raycast.h"
#include "scene.h"
#include "scene/scene.h"
#include "scene_type.h"
#include <stdlib.h>

typedef struct RaycastSceneData {
    Raycast raycast;
    RaycastScene_OnCollisionCallback onCollisionCallback;
    Scene* onCollisioonCallbackOwner;
} RaycastSceneData;

void Update(Scene* scene, double deltatime)
{
    RaycastSceneData* sceneData = scene->sceneData;

    if (sceneData->onCollisionCallback)
    {
        Vector2 point;
        if (Raycast_CheckForCollision(&sceneData->raycast, scene->globalTransform.position, &point))
            sceneData->onCollisionCallback(sceneData->onCollisioonCallbackOwner, point);
    }
}

Scene* RaycastScene_Create(Scene* parent, Vector2 direction, float length)
{
    Scene* scene = malloc(sizeof(Scene));
    Scene_DefaultInit(scene, SCENE_TYPE_RAYCAST);
    RaycastSceneData* sceneData = malloc(sizeof(RaycastSceneData));
    sceneData->raycast = Raycast_New(direction, length);
    sceneData->onCollisionCallback = NULL;
    scene->sceneData = sceneData;

    scene->updateFunction = Update;

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

    Scene_UpdateGlobalTransform(scene, false);

    Vector2 point;
    if (Raycast_CheckForCollision(&sceneData->raycast, scene->globalTransform.position, &point))
    {
        if (collisionPoint_out) *collisionPoint_out = point;
        return true;
    }

    return false;
}
