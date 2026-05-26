#ifndef __BUTTON_SCENE_H__
#define __BUTTON_SCENE_H__

#include "graphics/color.h"
#include "scene/scene.h"

typedef void (*ButtonScene_OnPressedCallback)(Scene* scene, i32 tag);

Scene* ButtonScene_Create(Scene* parent, const char* name, Vector2 size, Color color, const char* text, i32 tag, ButtonScene_OnPressedCallback callback, Scene* callbackOwner);
void ButtonScene_SetBorder(Scene* scene, float thickness, Color color);

#endif // __BUTTON_SCENE_H__