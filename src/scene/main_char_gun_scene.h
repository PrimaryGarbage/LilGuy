#ifndef __MAIN_CHAR_GUN_SCENE_H__
#define __MAIN_CHAR_GUN_SCENE_H__

#include "scene.h"

Scene* MainCharGunScene_Create(Scene* parent);
void MainCharGunScene_Shoot(Scene* scene, Vector2 gunSpeed);

#endif // __MAIN_CHAR_GUN_SCENE_H__