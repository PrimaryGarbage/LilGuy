#ifndef __RAYCAST_SCENE_H__
#define __RAYCAST_SCENE_H__

#include "scene.h"

typedef void (*RaycastScene_OnCollisionCallback)(Scene* scene, Vector2 collisionPoint);

Scene* RaycastScene_Create(Scene* parent, Vector2 direction, float length);
void RaycastScene_SetOnCollisionCallback(Scene* scene, Scene* callbackOwner, RaycastScene_OnCollisionCallback callback);
bool RaycastScene_CheckForCollision(Scene* scene, Vector2* collisionPoint_out);

#endif // __RAYCAST_SCENE_H__