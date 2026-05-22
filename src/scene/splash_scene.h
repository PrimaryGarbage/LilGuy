#ifndef __SPLASH_SCENE_H__
#define __SPLASH_SCENE_H__

#include "graphics/color.h"
#include "scene/scene.h"

Scene* SplashScene_Create(Scene* parent, Vector2 position, float scale, const char* name, Color color);

#endif // __SPLASH_SCENE_H__