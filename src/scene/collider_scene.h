#ifndef __COLLIDER_SCENE_H__
#define __COLLIDER_SCENE_H__

#include "scene.h"
#include "physics/collider.h"

typedef void (*ColliderScene_OnCollisionCallback)(Scene* scene, CollisionInfo info);

Scene* ColliderScene_Create(Scene* parent, Vector2 size, const char* name);
void ColliderScene_SetRect(Scene* scene, Rect rect);
void ColliderScene_SetOnCollisionCallback(Scene* scene, Scene* callbackOwner, ColliderScene_OnCollisionCallback callback);
void ColliderScene_SetVisible(Scene* scene, bool on);
void ColliderScene_SetCollisionLayers(Scene* scene, u32 layers);
void ColliderScene_SetCollisionScan(Scene* scene, u32 scan);
const Collider* ColliderScene_CheckForCollision(Scene* scene);
void ColliderScene_ForceUpdate(Scene* scene);

#endif // __COLLIDER_SCENE_H__