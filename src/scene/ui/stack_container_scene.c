#include "stack_container_scene.h"
#include "result.h"
#include "scene/scene.h"
#include <stdlib.h>

#define SCENE_TYPE SCENE_TYPE_STACK_CONTAINER
#define STACK_CONTAINER_SCENE_COUNT_MAX 64

typedef struct {
    float padding;
    bool horizontal;
} StackContainerSceneData;

static void Draw(Scene* scene)
{
    StackContainerSceneData* sceneData = scene->sceneData;

    float offset = 0.0f;
    for (u32 i = 0u; i < scene->childrenCount; ++i)
    {
        Scene* child = scene->children[i];

        if (sceneData->horizontal)
        {
            child->transform.position.x = offset;
            child->transform.position.y = 0.0f;

            offset += child->transform.size.x + sceneData->padding;
        }
        else
        {
            child->transform.position.x = 0.0f;
            child->transform.position.y = offset;

            offset += child->transform.size.y + sceneData->padding;
        }

        Scene_Draw(child);
    }
}

Scene* StackContainerScene_Create(Scene* parent, bool horizontal, float padding,const char* name)
{
    Scene* scene = malloc(sizeof(Scene));
    Scene_DefaultInit(scene, SCENE_TYPE_STACK_CONTAINER, parent, name);
    StackContainerSceneData* sceneData = malloc(sizeof(StackContainerSceneData));
    scene->sceneData = sceneData;

    sceneData->padding = padding;
    sceneData->horizontal = horizontal;

    scene->drawFunction = Draw;

    scene->drawChildren = false;

    return scene;
}