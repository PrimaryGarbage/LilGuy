#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "rect.h"

#define COLLIDER_LAYER_MAIN_CHAR 0b10
#define COLLIDER_LAYER_WORLD 0b1

typedef struct Collider {
    Rect rect;
    u32 layers;
    u32 scan;
} Collider;

typedef struct CollisionInfo {
    const Collider* collider;
    Rect collisionRect;
} CollisionInfo;

// return first colliding collider
Collider Collider_New(Rect rect);
const Collider* Collider_CheckForCollision(const Collider* collider);
const Collider* Collider_CheckForPointCollision(Vector2 point, u32 scan);
Rect Collider_GetCollisionRect(const Collider* colliderA, const Collider* colliderB);
void Collider_Register(const Collider* collider);
void Collider_Unregister(const Collider* collider);

#endif // __COLLIDER_H__