#ifndef __RAYCAST_H__
#define __RAYCAST_H__

#include "vector2.h"

typedef struct Raycast {
    Vector2 direction;
    float length;
    u32 scan;
} Raycast;

Raycast Raycast_New(Vector2 direction, float length);
bool Raycast_CheckForCollision(Raycast* raycast, Vector2 from, Vector2* collisionPoint_out);

#endif // __RAYCAST_H__