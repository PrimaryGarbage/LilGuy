#include "stack_container_scene.h"
#include "graphics/draw_order.h"
#include "graphics/graphics.h"
#include "math_helpers.h"
#include "rect.h"
#include "scene/scene.h"
#include <stdlib.h>

#define SCENE_TYPE SCENE_TYPE_STACK_CONTAINER
#define STACK_CONTAINER_SCENE_COUNT_MAX 64

typedef struct {
    float padding;
    bool horizontal;
    bool drawBackground;
    Color backgroundColor;
} StackContainerSceneData;

static void Update(Scene* scene, double deltatime)
{
    StackContainerSceneData* sceneData = scene->sceneData;

    float sizeX = 0.0f;
    float sizeY = 0.0f;

    if (sceneData->horizontal)
    {
        for(u32 i = 0u; i < scene->childrenCount; ++i)
        {
            sizeX += scene->children[i]->transform.size.x + sceneData->padding;
            sizeY = MAX(sizeY, scene->children[i]->transform.size.y + sceneData->padding);
        }
    }
    else
    {
        for(u32 i = 0u; i < scene->childrenCount; ++i)
        {
            sizeX = MAX(sizeX, scene->children[i]->transform.size.x + sceneData->padding);
            sizeY += scene->children[i]->transform.size.y + sceneData->padding;
        }
    }

    scene->transform.size = (Vector2){ sizeX, sizeY };
}

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

    if(sceneData->drawBackground)
        Graphics_DrawRect(Rect_FromVectors(scene->transform.position, scene->transform.size), sceneData->backgroundColor, DRAW_ORDER_UI - 1);
}

Scene* StackContainerScene_Create(Scene* parent, bool horizontal, float padding,const char* name)
{
    Scene* scene = malloc(sizeof(Scene));
    Scene_DefaultInit(scene, SCENE_TYPE_STACK_CONTAINER, parent, name);
    StackContainerSceneData* sceneData = malloc(sizeof(StackContainerSceneData));
    scene->sceneData = sceneData;

    sceneData->padding = padding;
    sceneData->horizontal = horizontal;
    sceneData->drawBackground = false;
    sceneData->backgroundColor = COLOR_NOCOLOR;

    scene->updateFunction = Update;
    scene->drawFunction = Draw;

    scene->drawChildren = false;

    return scene;
}

void StackContainerScene_SetBackground(Scene* scene, Color color)
{
    ASSERT_SCENE_TYPE(scene);
    StackContainerSceneData* sceneData = scene->sceneData;

    sceneData->backgroundColor = color;
    sceneData->drawBackground = true;
}