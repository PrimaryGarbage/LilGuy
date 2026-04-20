#include "collider.h"
#include "raylib_wrapper.h"
#include "result.h"
#include "logging.h"

#define STATIC_COLLIDER_COUNT_MAX 256

static const Collider* s_colliders[STATIC_COLLIDER_COUNT_MAX];

Collider Collider_New(Rect rect)
{
    return (Collider){
        .rect = rect,
        .layers = 1,
        .scan = 1
    };
}

const Collider* Collider_CheckForCollision(const Collider* collider)
{
    for(u32 i = 0; i < STATIC_COLLIDER_COUNT_MAX; ++i)
    {
        if(s_colliders[i] && s_colliders[i] != collider)
        {
            if (Raylib_CheckCollisionRects(collider->rect, s_colliders[i]->rect) && collider->scan & s_colliders[i]->layers)
            {
                return s_colliders[i];
            }
        }
    }

    return NULL;
}

const Collider* Collider_CheckForPointCollision(Vector2 point, u32 scan)
{
    for(u32 i = 0; i < STATIC_COLLIDER_COUNT_MAX; ++i)
    {
        if(s_colliders[i])
        {
            if (Raylib_CheckCollisionPointRect(point, s_colliders[i]->rect) && scan & s_colliders[i]->layers)
                return s_colliders[i];
        }
    }

    return NULL;
}

Rect Collider_GetCollisionRect(const Collider* colliderA, const Collider* colliderB)
{
    return Raylib_GetCollisionRect(colliderA->rect, colliderB->rect);
}

void Collider_Register(const Collider* collider)
{
    for(u32 i = 0; i < STATIC_COLLIDER_COUNT_MAX; ++i)
    {
        if(!s_colliders[i])
        {
            s_colliders[i] = collider;
            return;
        }
    }

    PANIC_EX(LogErrorM("Failed to register collider: max collider count exceeded"););
}

void Collider_Unregister(const Collider* collider)
{
    for(u32 i = 0; i < STATIC_COLLIDER_COUNT_MAX; ++i)
    {
        if(s_colliders[i] && collider == s_colliders[i])
        {
            s_colliders[i] = NULL;
            return;
        }
    }
}