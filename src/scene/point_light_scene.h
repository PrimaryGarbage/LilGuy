#ifndef __POINT_LIGHT_SCENE_H__
#define __POINT_LIGHT_SCENE_H__

#include "graphics/color.h"
#include "scene.h"

Scene* PointLightScene_Create(Scene* parent, const char* name, Color lightColor, float lightRadius);
void PointLightScene_SetLight(Scene* scene, Color color, float radius);

#endif // __POINT_LIGHT_SCENE_H__