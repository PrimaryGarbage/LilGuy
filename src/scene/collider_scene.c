#include "collider_scene.h"
#include "graphics/graphics.h"
#include "scene_type.h"
#include <stdlib.h>

typedef struct ColliderSceneData {
    Collider collider;
    OnCollisionCallback onCollisionCallback;
    bool enabled;
    bool visible;
} ColliderSceneData;

static void Start(Scene* scene)
{
    ColliderSceneData* sceneData = scene->sceneData;

    Collider_Register(&sceneData->collider);
}

static void Update(Scene* scene, double deltatime)
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
            sceneData->onCollisionCallback(scene->parent, (CollisionInfo) { 
                .collider = otherCollider, 
                .collisionRect = Collider_GetCollisionRect(&sceneData->collider, otherCollider)
            });
        }
    }
}

static void Draw(Scene* scene)
{
    constexpr Color colliderColor = { .r = 100, .g = 20, .b = 20, .a = 100 };

    ColliderSceneData* sceneData = scene->sceneData;

    if(!sceneData->visible) return;

    Graphics_SetTransform(&scene->globalTransform);
    Graphics_DrawRectT((Vector2){ sceneData->collider.rect.width, sceneData->collider.rect.height }, colliderColor);
    Graphics_ClearTransform();
}

static void Cleanup(Scene* scene)
{
    ColliderSceneData* sceneData = scene->sceneData;

    Collider_Unregister(&sceneData->collider);
}

Scene* ColliderScene_Create(Scene* parent, Vector2 size)
{
    Scene* scene = malloc(sizeof(Scene));

    ColliderSceneData* sceneData = malloc(sizeof(ColliderSceneData));
    sceneData->visible = false;
    sceneData->enabled = true;
    sceneData->onCollisionCallback = NULL;
    sceneData->collider = Collider_New((Rect){
        .width = size.x,
        .height = size.y
    });

    scene->sceneData = sceneData;
    scene->type = SCENE_TYPE_COLLIDER;
    scene->transform.position = Vector2_Zero();
    scene->transform.scale = Vector2_New(1.0f, 1.0f);
    scene->transform.origin = Vector2_Zero();
    scene->transform.rotation = 0.0f;
    scene->transform.topLevel = false;
    scene->childrenCount = 0u;
    scene->parent = parent;

    Scene_UpdateGlobalTransform(scene, false);

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

void ColliderScene_SetOnCollisionCallback(Scene* scene, OnCollisionCallback callback)
{
    ASSERT_SCENE_TYPE(scene, SCENE_TYPE_COLLIDER);

    ((ColliderSceneData*)scene->sceneData)->onCollisionCallback = callback;
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