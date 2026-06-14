#ifndef __GAME_MANAGER_H__
#define __GAME_MANAGER_H__

#include "scene/scene.h"

void GameManager_Init(Scene* root);
Scene* GameManager_GetRootScene();
Scene* GameManager_GetBulletParentScene();
Scene* GameManager_GetGenericSpawnParentScene();
Scene* GameManager_GetMainCharScene();
void GameManager_RequestExit();
bool GameManager_IsExitRequested();
void GameManager_SetDeltatime(double deltatime);
double GameManager_GetDeltatime();
void GameManager_PauseGame(bool on);

#endif // __GAME_MANAGER_H__