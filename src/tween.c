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
    Tween_Interpolation interpolation;
    bool active;
} Tween;

static Tween s_tweens[TWEEN_MAX_INSTANCES];

static double ApplyInterpolation(double weight, Tween_Interpolation interpolation)
{
    switch(interpolation)
    {
        case TWEEN_INTERPOLATION_LINEAR: 
            return weight;
        case TWEEN_INTERPOLATION_QUADRATIC:
            return weight * weight;
        case TWEEN_INTERPOLATION_CUBIC:
            return weight * weight * weight;
        case TWEEN_INTERPOLATION_QUADRATIC_EASE_OUT:
        {
            double num = 1.0 - weight;
            return 1.0 - num * num;
        }
        case TWEEN_INTERPOLATION_CUBIC_EASE_OUT:
        {
            double num = 1.0 - weight;
            return 1.0 - num * num * num;
        }
    }
}

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
            if (tween->function)
            {
                tween->function(tween->functionOwner, ApplyInterpolation(tween->elapsed / tween->limit, tween->interpolation), tween->elapsed);
            }
        }
    }
}

TweenHandle Tween_CreateFunction(double timeLimit, Scene* functionOwner, TweenFunction function, Tween_Interpolation interpolation)
{
    for(u32 i = 0u; i < TWEEN_MAX_INSTANCES; ++i)
    {
        if (s_tweens[i].active) continue;
        
        s_tweens[i] = (Tween){
            .elapsed = 0.0,
            .limit = timeLimit,
            .function = function,
            .functionOwner = functionOwner,
            .onFinish = NULL,
            .onFinishCallbackOwner = NULL,
            .active = true,
            .interpolation = interpolation,
        };
        
        return &s_tweens[i];
    }
    
    PANIC_EX(LogErrorM("Failed to create tween function: Unable to find inactive tween."););
}

void Tween_SetOnFinishCallback(TweenHandle tween, Scene* callbackOwner, TweenOnFinishCallback callback)
{
    tween->onFinish = callback;
    tween->onFinishCallbackOwner = callbackOwner;
}

void Tween_CreateTimer(double timeLimit, Scene* callbackOwner, TweenOnFinishCallback callback)
{
    for(u32 i = 0u; i < TWEEN_MAX_INSTANCES; ++i)
    {
        if (s_tweens[i].active) continue;

        s_tweens[i] = (Tween){
            .elapsed = 0.0,
            .limit = timeLimit,
            .function = NULL,
            .functionOwner = NULL,
            .onFinish = callback,
            .onFinishCallbackOwner = callbackOwner,
            .active = true
        };

        return;
    }
    
    PANIC_EX(LogErrorM("Failed to create tween function: Unable to find inactive tween."););
}

void Tween_Stop(TweenHandle tween)
{
    tween->active = false;
}