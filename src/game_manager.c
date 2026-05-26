#include "game_manager.h"
#include "result.h"
#include "scene/empty_scene.h"
#include "scene/scene_type.h"

static Scene* s_rootScene;
static Scene* s_mainCharScene;
static Scene*  s_bulletParentScene;

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

Scene* GameManager_GetMainCharScene()
{
    if (!s_mainCharScene) PANIC_M("Main Char scene isn't set in the Game Manager!");

    return s_mainCharScene;
}
