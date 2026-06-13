#ifndef __LABEL_SCENE_H__
#define __LABEL_SCENE_H__

#include "graphics/color.h"
#include "scene/scene.h"

Scene* LabelScene_Create(Scene* parent, const char* name, const char* text, float fontSize, Color textColor);
void LabelScene_SetText(Scene* scene, const char* text);

#endif // __LABEL_SCENE_H__