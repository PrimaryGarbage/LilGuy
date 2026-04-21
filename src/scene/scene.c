#include "scene.h"
#include "logging.h"
#include "physics/transform.h"
#include "result.h"
#include <stdlib.h>
#include <string.h>

static Scene* s_scenesToFree[512];
static u32 s_scenesToFreeCount = 0u;

static u32 s_idCounter = 0u;

void Scene_Free(Scene* scene)
{
    LogInfo("Scene freed: %s. ID: %d", scene->name, scene->id);

    Scene_Cleanup(scene);

    if(scene->parent)
        Scene_RemoveChild(scene->parent, scene);

    for(u32 i = 0u; i < scene->childrenCount; ++i)
        Scene_Free(scene->children[i]);

    if (scene->sceneData)
        free(scene->sceneData);

    free(scene);
}

void Scene_AddChild(Scene* scene, Scene* child)
{
    if (scene->childrenCount == SCENE_CHILDREN_MAX)
        PANIC_EX(LogErrorM("Exceeded the max number of children for the scene"););
    if(child->parent)
        PANIC_EX(LogErrorM("Child scene already has a parent! Make sure that you aren't adding the same child twice or forgot to unparent the child first"););

    scene->children[scene->childrenCount++] = child;
    child->parent = scene;
}

void Scene_RemoveChild(Scene* scene, Scene* child)
{
    for(u32 i = 0u; i < scene->childrenCount; ++i)
    {
        if (scene->children[i]->id == child->id)
        {
            for(u32 j = i; j < scene->childrenCount - 1; ++j)
            {
                scene->children[j] = scene->children[j + 1];
            }

            scene->childrenCount--;
            child->parent = NULL;
            return;
        }
    }
}

void Scene_UpdateGlobalTransform(Scene* scene)
{
    Transform_UpdateMatrix(&scene->transform);

    if (!scene->parent || scene->transform.topLevel)
    {
        scene->globalTransform = scene->transform;
        return;
    }

    Matrix globalMatrix = Matrix_Mult(&scene->transform.matrix, &scene->parent->globalTransform.matrix);
    scene->globalTransform = Transform_FromMatrix(&globalMatrix);
    scene->globalTransform.origin = scene->transform.origin;
}

static void Scene_StartChildren(Scene* scene)
{
    for(u32 i = 0; i < scene->childrenCount; ++i)
        Scene_Start(scene->children[i]);
}

static void Scene_UpdateChildren(Scene* scene, double deltatime)
{
    for(u32 i = 0; i < scene->childrenCount; ++i)
        Scene_Update(scene->children[i], deltatime);
}

static void Scene_DrawChildren(Scene* scene)
{
    for(u32 i = 0; i < scene->childrenCount; ++i)
        Scene_Draw(scene->children[i]);
}

void Scene_Update(Scene* scene, double deltatime)
{
    if (scene->updateFunction)
    {
        Scene_UpdateGlobalTransform(scene);
        scene->updateFunction(scene, deltatime);
    }

    Scene_UpdateGlobalTransform(scene);

    Scene_UpdateChildren(scene, deltatime);
}

void Scene_Draw(Scene* scene)
{
    if (!scene->visible) return;

    if (scene->drawFunction) scene->drawFunction(scene);

    Scene_DrawChildren(scene);
}

void Scene_DefaultInit(Scene* scene, SceneType type, Scene* parent, const char* name)
{
    scene->sceneData = NULL;
    scene->id = Scene_GenerateId();
    scene->type = type;
    scene->childrenCount = 0u;
    scene->transform = Transform_Zero();
    scene->visible = true;
    scene->name = name;
    scene->parent = NULL;

    scene->startFunction = NULL;
    scene->updateFunction = NULL;
    scene->drawFunction = NULL;
    scene->cleanupFunction = NULL;

    if (parent) Scene_AddChild(parent, scene);

    Scene_UpdateGlobalTransform(scene);
}

void Scene_QueueFree(Scene* scene)
{
    s_scenesToFree[s_scenesToFreeCount++] = scene;
}

u32 Scene_GenerateId()
{
    return s_idCounter++;
}

Scene* Scene_GetRoot(Scene* scene)
{
    while(scene->parent)
        scene = scene->parent;

    return scene;
}

void Scene_Cleanup(Scene* scene)
{
    if (scene->cleanupFunction) scene->cleanupFunction(scene);
}

void Scene_Start(Scene* scene)
{
    if (scene->startFunction) scene->startFunction(scene);
    Scene_StartChildren(scene);
}

void Scene_TrimQueuedScenes()
{
    for(u32 i = 0u; i < s_scenesToFreeCount; ++i)
        Scene_Free(s_scenesToFree[i]);

    s_scenesToFreeCount = 0u;
}