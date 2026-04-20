#ifndef __RAYCAST_SCENE_H__
#define __RAYCAST_SCENE_H__

#include "scene.h"

typedef void (*RaycastScene_OnCollisionCallback)(Scene* scene, Vector2 collisionPoint);

Scene* RaycastScene_Create(Scene* parent, Vector2 direction, float length, const char* name);
void RaycastScene_SetOnCollisionCallback(Scene* scene, Scene* callbackOwner, RaycastScene_OnCollisionCallback callback);
bool RaycastScene_CheckForCollision(Scene* scene, Vector2* collisionPoint_out);
void RaycastScene_SetVisible(Scene* scene, bool on);
void RaycastScene_SetLength(Scene* scene, float length);

#endif // __RAYCAST_SCENE_H__