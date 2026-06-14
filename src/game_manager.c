#include "game_manager.h"
#include "result.h"
#include "scene/empty_scene.h"
#include "scene/scene_type.h"

#define GAME_PAUSE_RESERVATION_COUNT_MAX 8

static Scene* s_rootScene;
static Scene* s_mainCharScene;
static Scene*  s_bulletParentScene;
static Scene* s_genericSpawnParentScene;
static bool s_exitRequested = false;
static double s_deltatime;
static bool s_gamePauseReservations[GAME_PAUSE_RESERVATION_COUNT_MAX];

static bool IsGamePaused()
{
    for (u32 i = 0u; i < GAME_PAUSE_RESERVATION_COUNT_MAX; ++i)
    {
        if (s_gamePauseReservations[i])
            return true;
    }

    return false;
}

static Scene* SearchMainCharScene(Scene* parent)
{
    if (parent->type == SCENE_TYPE_MAIN_CHAR)
        return parent;

    for (u32 i = 0u; i < parent->childrenCount; ++i)
    {
        if (parent->children[i]->type == SCENE_TYPE_MAIN_CHAR)
            return parent->children[i];
    }

    for (u32 i = 0u; i < parent->childrenCount; ++i)
    {
        Scene* found = SearchMainCharScene(parent->children[i]);
        if (found) return found;
    }

    return NULL;
}

void GameManager_Init(Scene* root)
{
    s_rootScene = root;

    s_mainCharScene = SearchMainCharScene(root);

    s_bulletParentScene = EmptyScene_Create(root, "Bullet Parent");
    s_genericSpawnParentScene = EmptyScene_Create(root, "Generic Spawn Parent");

    if (!s_mainCharScene)
        PANIC_M("Failed to initialize Game Manager: Failed to find main char scene among the root scene children");
}

Scene* GameManager_GetRootScene()
{
    if (!s_rootScene) PANIC_M("Root scene isn't set in the Game Manager!");

    return s_rootScene;
}

Scene* GameManager_GetBulletParentScene()
{
    return s_bulletParentScene;
}

Scene* GameManager_GetGenericSpawnParentScene()
{
    return s_genericSpawnParentScene;
}

Scene* GameManager_GetMainCharScene()
{
    if (!s_mainCharScene) PANIC_M("Main Char scene isn't set in the Game Manager!");

    return s_mainCharScene;
}

void GameManager_RequestExit()
{
    for (u32 i = 0u; i < GAME_PAUSE_RESERVATION_COUNT_MAX; ++i)
        s_gamePauseReservations[i] = false;

    s_exitRequested = true;
}

bool GameManager_IsExitRequested()
{
    return s_exitRequested;
}

void GameManager_SetDeltatime(double deltatime)
{
    s_deltatime = deltatime;
}

double GameManager_GetDeltatime()
{
    return IsGamePaused()? 0.0 : s_deltatime;
}

u32 GameManager_RequestGamePause()
{
    for(u32 i = 0u; i < GAME_PAUSE_RESERVATION_COUNT_MAX; ++i)
    {
        if (s_gamePauseReservations[i]) continue;

        s_gamePauseReservations[i] = true;
        return i;
    }

    PANIC_M("Max number of game pause reservations exceeded!");
}

void GameManager_RemoveGamePauseRequest(u32 tag)
{
    s_gamePauseReservations[tag] = false;
}