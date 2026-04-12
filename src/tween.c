#include "tween.h"
#include "logging.h"
#include "result.h"
#include "short_types.h"

#define TWEEN_MAX_INSTANCES 100

typedef struct Tween {
    double elapsed;
    double limit;
    TweenFunction function;
    Scene* functionOwner;
    TweenOnFinishCallback onFinish;
    Scene* onFinishCallbackOwner;
    bool active;
} Tween;

static Tween s_tweens[TWEEN_MAX_INSTANCES];

void Tween_Update(double deltatime)
{
    for(u32 i = 0; i < TWEEN_MAX_INSTANCES; ++i)
    {
        Tween* tween = &s_tweens[i];
        if (!tween->active) continue;

        tween->elapsed += deltatime;

        if (tween->elapsed > tween->limit)
        {
            if (tween->onFinish)
                tween->onFinish(tween->onFinishCallbackOwner);

            tween->active = false;
        }
        else
        {
            tween->function(tween->functionOwner, tween->elapsed);
        }
    }
}

TweenHandle Tween_CreateFunction(double timeLimit, Scene* functionOwner, TweenFunction function)
{
    for(u32 i = 0u; i < TWEEN_MAX_INSTANCES; ++i)
    {
        if (!s_tweens[i].active)
        {
            s_tweens[i] = (Tween){
                .elapsed = 0.0,
                .limit = timeLimit,
                .function = function,
                .functionOwner = functionOwner,
                .onFinish = NULL,
                .onFinishCallbackOwner = NULL,
                .active = true
            };
            
            return &s_tweens[i];
        }
    }
    
    PANIC_EX(LogErrorM("Failed to create tween function: Unable to find inactive tween."););
}

void Tween_SetOnFinishCallback(TweenHandle tween, Scene* callbackOwner, TweenOnFinishCallback callback)
{
    tween->onFinish = callback;
    tween->onFinishCallbackOwner = callbackOwner;
}