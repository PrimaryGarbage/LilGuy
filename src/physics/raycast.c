#include "raycast.h"
#include "vector2.h"
#include "collider.h"
#include <math.h>

Raycast Raycast_New(Vector2 direction, float length)
{
    direction = Vector2_Normalize(direction);
    
    return (Raycast){
        .direction = direction,
        .length = length,
        .scan = 1
    };
}

bool Raycast_CheckForCollision(Raycast* raycast, Vector2 from, Vector2* collisionPoint_out)
{
    constexpr float step = 0.1f;

    double distance = 0.0;
    Vector2 ray = from;
    const Collider* collider = NULL;

    while(distance < raycast->length)
    {
        distance += step;
        ray = Vector2_Add(ray, Vector2_MultScalar(raycast->direction, step));
        collider = Collider_CheckForPointCollision(ray, raycast->scan);
        if (collider)
        {
            if(collisionPoint_out) *collisionPoint_out = ray;
            return true;
        }
    }

    return false;
}