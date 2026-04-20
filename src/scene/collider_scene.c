#include "collider_scene.h"
#include "graphics/draw_order.h"
#include "graphics/graphics.h"
#include "scene.h"
#include "scene_type.h"
#include <stdlib.h>

typedef struct ColliderSceneData {
    Collider collider;
    ColliderScene_OnCollisionCallback onCollisionCallback;
    Scene* onCollisionCallbackOwner;
    bool enabled;
    bool visible;
} ColliderSceneData;

static void Start(Scene* scene)
{
    ColliderSceneData* sceneData = scene->sceneData;

    Collider_Register(&sceneData->collider);
}

static void SearchCollisions(Scene* scene)
{
    ColliderSceneData* sceneData = scene->sceneData;

    // Update collider position first
    sceneData->collider.rect.x = scene->globalTransform.position.x;
    sceneData->collider.rect.y = scene->globalTransform.position.y;

    if (sceneData->onCollisionCallback)
    {
        const Collider* otherCollider = Collider_CheckForCollision(&sceneData->collider);
        if (otherCollider)
        {
            sceneData->onCollisionCallback(sceneData->onCollisionCallbackOwner, (CollisionInfo) { 
                .collider = otherCollider, 
                .collisionRect = Collider_GetCollisionRect(&sceneData->collider, otherCollider)
            });

            return;
        }
    }
}

static void Update(Scene* scene, double _)
{
    SearchCollisions(scene);
}

static void Draw(Scene* scene)
{
    constexpr Color colliderColor = { .r = 100, .g = 20, .b = 20, .a = 200 };

    ColliderSceneData* sceneData = scene->sceneData;

    if(!sceneData->visible) return;

    Graphics_SetModelMatrix(&scene->globalTransform);
    Graphics_DrawRectT((Vector2){ sceneData->collider.rect.width, sceneData->collider.rect.height }, colliderColor, DRAW_ORDER_TOP);
    Graphics_ClearModelMatrix();
}

static void Cleanup(Scene* scene)
{
    ColliderSceneData* sceneData = scene->sceneData;

    Collider_Unregister(&sceneData->collider);
}

Scene* ColliderScene_Create(Scene* parent, Vector2 size, const char* name)
{
    Scene* scene = malloc(sizeof(Scene));
    Scene_DefaultInit(scene, SCENE_TYPE_COLLIDER, name);
    if (parent) Scene_AddChild(parent, scene);

    ColliderSceneData* sceneData = malloc(sizeof(ColliderSceneData));
    sceneData->visible = false;
    sceneData->enabled = true;
    sceneData->onCollisionCallback = NULL;
    sceneData->onCollisionCallbackOwner = NULL;
    sceneData->collider = Collider_New((Rect){
        .width = size.x,
        .height = size.y
    });

    scene->sceneData = sceneData;

    Scene_UpdateGlobalTransform(scene);

    scene->startFunction = Start;
    scene->updateFunction = Update;
    scene->drawFunction = Draw;
    scene->cleanupFunction = Cleanup;

    return scene;
}

void ColliderScene_SetRect(Scene* scene, Rect rect)
{
    ASSERT_SCENE_TYPE(scene, SCENE_TYPE_COLLIDER);

    ((ColliderSceneData*)scene->sceneData)->collider.rect = rect;
}

void ColliderScene_SetOnCollisionCallback(Scene* scene, Scene* callbackOwner, ColliderScene_OnCollisionCallback callback)
{
    ASSERT_SCENE_TYPE(scene, SCENE_TYPE_COLLIDER);

    ColliderSceneData* sceneData = scene->sceneData;

    sceneData->onCollisionCallback = callback;
    sceneData->onCollisionCallbackOwner = callbackOwner;
}

void ColliderScene_SetVisible(Scene* scene, bool on)
{
    ASSERT_SCENE_TYPE(scene, SCENE_TYPE_COLLIDER);

    ((ColliderSceneData*)scene->sceneData)->visible = on;
}

void ColliderScene_SetCollisionLayers(Scene* scene, u32 layers)
{
    ASSERT_SCENE_TYPE(scene, SCENE_TYPE_COLLIDER);

    ((ColliderSceneData*)scene->sceneData)->collider.layers = layers;
}

void ColliderScene_SetCollisionScan(Scene* scene, u32 scan)
{
    ASSERT_SCENE_TYPE(scene, SCENE_TYPE_COLLIDER);

    ((ColliderSceneData*)scene->sceneData)->collider.scan = scan;
}

const Collider* ColliderScene_CheckForCollision(Scene* scene)
{
    ASSERT_SCENE_TYPE(scene, SCENE_TYPE_COLLIDER);

    ColliderSceneData* sceneData = scene->sceneData;

    // Update collider position first
    sceneData->collider.rect.x = scene->globalTransform.position.x;
    sceneData->collider.rect.y = scene->globalTransform.position.y;

    if (sceneData->onCollisionCallback)
    {
        const Collider* otherCollider = Collider_CheckForCollision(&sceneData->collider);
        if (otherCollider) return otherCollider;
    }

    return NULL;
}

void ColliderScene_ForceUpdate(Scene* scene)
{
    Scene_UpdateGlobalTransform(scene);
    SearchCollisions(scene);
}