#ifndef __ANIMATED_SPRITE_SCENE_H__
#define __ANIMATED_SPRITE_SCENE_H__

#include "graphics/color.h"
#include "scene.h"

typedef void (*AnimatedSpriteScene_OnAnimationFinishCallback)(Scene* scene);

Scene* AnimatedSpriteScene_Create(Scene* parent, const char* imagePath, u32 atlasWidth, u32 atlasHeight, double switchTime, u32 drawOrder, const char* sceneName);
Vector2 AnimatedSpriteScene_GetSpriteSize(const Scene* scene);
void AnimatedSpriteScene_SetTint(Scene* scene, Color tint);
void AnimatedSpriteScene_SetOnAnimationFinishCallback(Scene* scene, AnimatedSpriteScene_OnAnimationFinishCallback callback, Scene* callbackOwner);
void AnimatedSpriteScene_SetOriginToCenter(Scene* scene);

#endif // __ANIMATED_SPRITE_SCENE_H__