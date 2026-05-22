#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "rect.h"

// FD
typedef struct Scene Scene;

typedef struct Collider {
    Rect rect;
    u32 layers;
    u32 scan;
    Scene* owner;
} Collider;

Collider Collider_New(Rect rect, Scene* owner);
const Collider* Collider_CheckForCollision(const Collider* collider);
const Collider* Collider_CheckForPointCollision(Vector2 point, u32 scan);
Rect Collider_GetCollisionRect(const Collider* colliderA, const Collider* colliderB);
void Collider_Register(const Collider* collider);
void Collider_Unregister(const Collider* collider);

#endif // __COLLIDER_H__