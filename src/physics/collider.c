#include "collider.h"
#include "raylib_wrapper.h"
#include "result.h"
#include "logging.h"
#include "graphics/graphics.h"

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
            if (Raylib_CheckCollisionRects(Graphics_WorldToScreenRect(collider->rect), Graphics_WorldToScreenRect(s_colliders[i]->rect)) && collider->scan & s_colliders[i]->layers)
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
            if (Raylib_CheckCollisionPointRect(Graphics_WorldToScreen(point), Graphics_WorldToScreenRect(s_colliders[i]->rect)) && scan & s_colliders[i]->layers)
                return s_colliders[i];
        }
    }

    return NULL;
}

Rect Collider_GetCollisionRect(const Collider* colliderA, const Collider* colliderB)
{
    return Graphics_ScreenToWorldRect(Raylib_GetCollisionRect(Graphics_WorldToScreenRect(colliderA->rect), Graphics_WorldToScreenRect(colliderB->rect)));
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