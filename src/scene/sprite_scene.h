#ifndef __SPRITE_SCENE_H__
#define __SPRITE_SCENE_H__

#include "graphics/texture2d.h"
#include "scene.h"

Scene* SpriteScene_Create(Scene* parent, const char* imagePath, const char* name);
Scene* SpriteScene_CreateWithTexture(Scene* parent, const Texture2D* texture, const char* name);
void SpriteScene_SetDrawOrder(Scene* scene, i32 drawOrder);

#endif // __SPRITE_SCENE_H__