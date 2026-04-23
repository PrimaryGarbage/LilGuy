#ifndef __SPRING_ARM_SCENE_H__
#define __SPRING_ARM_SCENE_H__

#include "scene.h"

Scene* SpringArmScene_Create(Scene* parent, Scene* target, Vector2 initialOffset, float stiffness, const char* name);

#endif // __SPRING_ARM_SCENE_H__