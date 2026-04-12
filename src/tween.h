#ifndef __TWINE_H__
#define __TWINE_H__

#include "scene/scene.h"

typedef struct Tween* TweenHandle;

typedef void(*TweenFunction)(Scene* functionOwner, double elapsed);
typedef void(*TweenOnFinishCallback)(Scene* callbackOwner);

void Tween_Update(double deltatime);
TweenHandle Tween_CreateFunction(double timeLimit, Scene* functionOwner, TweenFunction function);
void Tween_SetOnFinishCallback(TweenHandle tween, Scene* callbackOwner, TweenOnFinishCallback callback);

#endif // __TWINE_H__