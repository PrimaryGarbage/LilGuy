#ifndef __TWINE_H__
#define __TWINE_H__

#include "scene/scene.h"

typedef struct Tween* TweenHandle;

typedef enum Tween_Interpolation{
    TWEEN_INTERPOLATION_LINEAR,
    TWEEN_INTERPOLATION_QUADRATIC,
    TWEEN_INTERPOLATION_CUBIC,
    TWEEN_INTERPOLATION_QUADRATIC_EASE_OUT,
    TWEEN_INTERPOLATION_CUBIC_EASE_OUT,
} Tween_Interpolation;

typedef void(*TweenFunction)(Scene* functionOwner, double weight, double elapsed);
typedef void(*TweenOnFinishCallback)(Scene* callbackOwner);

void Tween_Update(double deltatime);
TweenHandle Tween_CreateFunction(double timeLimit, Scene* functionOwner, TweenFunction function, Tween_Interpolation interpolation);
void Tween_SetOnFinishCallback(TweenHandle tween, Scene* callbackOwner, TweenOnFinishCallback callback);
void Tween_CreateTimer(double timeLimit, Scene* callbackOwner, TweenOnFinishCallback callback);
void Tween_Abort(TweenHandle tween);

#endif // __TWINE_H__