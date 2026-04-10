#ifndef __BLOCK_SCENE_H__
#define __BLOCK_SCENE_H__

#include "graphics/color.h"
#include "scene.h"
#include "rect.h"

Scene* BlockScene_Create(Scene* parent, Rect rect, Color color);

#endif // __BLOCK_SCENE_H__