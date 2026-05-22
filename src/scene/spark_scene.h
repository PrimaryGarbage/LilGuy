#ifndef __SPARK_SCENE_H__
#define __SPARK_SCENE_H__

#include "scene.h"
#include "graphics/color.h"

Scene* SparkScene_Create(Scene* parent, Vector2 position, float scale, Color color, bool lightOn, Color lightColor, const char* name);

#endif // __SPARK_SCENE_H__