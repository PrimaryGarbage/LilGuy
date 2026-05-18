#ifndef __RAYCAST_H__
#define __RAYCAST_H__

#include "vector2.h"

// FD
typedef struct Collider Collider;

typedef struct Raycast {
    Vector2 position;
    Vector2 direction;
    float length;
    u32 scan;
} Raycast;

Raycast Raycast_New(Vector2 position, Vector2 direction, float length, u32 scan);
const Collider* Raycast_CheckForCollision(Raycast* raycast, Vector2* collisionPoint_out);

#endif // __RAYCAST_H__