#include "scene.h"
#include "logging.h"
#include "result.h"
#include <stdlib.h>

void Scene_Free(Scene* scene)
{
    Scene_Cleanup(scene);

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

    scene->children[scene->childrenCount++] = child;
    child->parent = scene;
}

void Scene_UpdateGlobalTransform(Scene* scene, bool recurse)
{
    if (!scene->parent || scene->transform.topLevel)
    {
        scene->globalTransform = scene->transform;
        return;
    }

    if (recurse)
        Scene_UpdateGlobalTransform(scene->parent, recurse);

    scene->globalTransform = Transform_Combine(&scene->parent->globalTransform, &scene->transform);
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

static void Scene_CleanupChildren(Scene* scene)
{
    for(u32 i = 0; i < scene->childrenCount; ++i)
        Scene_Cleanup(scene->children[i]);
}

void Scene_Update(Scene* scene, double deltatime)
{
    if (scene->updateFunction)
    {
        Scene_UpdateGlobalTransform(scene, false);
        scene->updateFunction(scene, deltatime);
    }

    Scene_UpdateGlobalTransform(scene, false);


    Scene_UpdateChildren(scene, deltatime);
}

void Scene_Draw(Scene* scene)
{
    if (!scene->visible) return;

    if (scene->drawFunction) scene->drawFunction(scene);
    Scene_DrawChildren(scene);
}

void Scene_DefaultInit(Scene* scene, SceneType type, const char* name)
{
    scene->sceneData = NULL;
    scene->type = type;
    scene->childrenCount = 0u;
    scene->transform = Transform_Zero();
    scene->visible = true;
    scene->name = name;

    scene->startFunction = NULL;
    scene->updateFunction = NULL;
    scene->drawFunction = NULL;
    scene->cleanupFunction = NULL;
    scene->parent = NULL;

    Scene_UpdateGlobalTransform(scene, false);
}

void Scene_Cleanup(Scene* scene)
{
    if (scene->cleanupFunction) scene->cleanupFunction(scene);
    Scene_CleanupChildren(scene);
}

void Scene_Start(Scene* scene)
{
    if (scene->startFunction) scene->startFunction(scene);
    Scene_StartChildren(scene);
}