#ifndef __RAYCAST_SCENE_H__
#define __RAYCAST_SCENE_H__

#include "scene.h"

typedef void (*RaycastScene_OnCollisionCallback)(Scene* scene, Vector2 collisionPoint);

Scene* RaycastScene_Create(Scene* parent, Vector2 direction, float length, const char* name);
void RaycastScene_SetOnCollisionCallback(Scene* scene, Scene* callbackOwner, RaycastScene_OnCollisionCallback callback);
bool RaycastScene_CheckForCollision(Scene* scene, Vector2* collisionPoint_out);
void RaycastScene_SetVisible(Scene* scene, bool on);
void RaycastScene_SetLength(Scene* scene, float length);
float RaycastScene_GetLength(Scene* scene);
Vector2 RaycastScene_GetRaycastVector(Scene* scene);
// Return point in global coordinates to which the raycast points
Vector2 RaycastScene_GetGlobalRaycastPointToVector(Scene* scene);

#endif // __RAYCAST_SCENE_H__