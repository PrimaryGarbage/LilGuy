#ifndef __STACK_CONTAINER_SCENE_H__
#define __STACK_CONTAINER_SCENE_H__

#include "graphics/color.h"
#include "scene/scene.h"

Scene* StackContainerScene_Create(Scene* parent, bool horizontal, float padding, const char* name);
void StackContainerScene_SetBackground(Scene* scene, Color color);

#endif // __STACK_CONTAINER_SCENE_H__