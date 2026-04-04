#include "scene.h"
#include "logging.h"
#include "result.h"
#include <stdlib.h>

void Scene_Destroy(Scene* scene)
{
    for(u32 i = 0u; i < scene->childrenCount; ++i)
        Scene_Destroy(scene->children[i]);

    if (scene->customData)
        free(scene->customData);

    free(scene);
}

void Scene_AddChild(Scene* scene, Scene* child)
{
    if (scene->childrenCount == SCENE_CHILDREN_MAX)
        PANIC_EX(LogErrorM("Exceeded the max number of children for the scene"););

    scene->children[scene->childrenCount++] = child;
    child->parent = scene;
}