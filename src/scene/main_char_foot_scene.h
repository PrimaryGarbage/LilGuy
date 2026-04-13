#ifndef __MAIN_CHAR_FOOT_SCENE_H__
#define __MAIN_CHAR_FOOT_SCENE_H__

#include "scene.h"

typedef Vector2(*GetNewFootPositionCallback)(Scene* scene);

Scene* MainCharFootScene_Create(Scene* parent);
void MainCharFootScene_MoveFoot(Scene* scene, Vector2 point);
bool MainCharFootScene_InAnimation(Scene* scene);

#endif // __MAIN_CHAR_FOOT_SCENE_H__