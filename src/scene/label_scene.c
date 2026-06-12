#include "label_scene.h"
#include "graphics/draw_order.h"
#include "graphics/graphics.h"
#include "scene.h"
#include "raylib_wrapper.h"

#include <stdlib.h>
#include <string.h>

#define SCENE_TYPE SCENE_TYPE_LABEL

typedef struct {
    char* buffer;
    size_t bufferSize;
    float fontSize;
    Color color;
} LabelSceneData;

static void Draw(Scene* scene)
{
    LabelSceneData* sceneData = scene->sceneData;

    Graphics_DrawText(sceneData->buffer, scene->globalTransform.position, sceneData->fontSize, sceneData->color, DRAW_ORDER_UI);
}

static void Cleanup(Scene* scene)
{
    LabelSceneData* sceneData = scene->sceneData;
    free(sceneData->buffer);
}

Scene* LabelScene_Create(Scene* parent, const char* name, const char* text, float fontSize, Color textColor)
{
    Scene* scene = malloc(sizeof(Scene));
    Scene_DefaultInit(scene, SCENE_TYPE_LABEL, parent, name);
    LabelSceneData* sceneData = malloc(sizeof(LabelSceneData));
    scene->sceneData = sceneData;

    scene->transform.size = Raylib_MeasureText(text, fontSize);

    size_t textLength = strlen(text);
    sceneData->bufferSize = textLength + 1;
    sceneData->buffer = malloc(sceneData->bufferSize);
    strcpy(sceneData->buffer, text);
    sceneData->fontSize = fontSize;
    sceneData->color = textColor;

    scene->drawFunction = Draw;
    scene->cleanupFunction = Cleanup;

    return scene;
}

void LabelScene_SetText(Scene* scene, const char* text)
{
    ASSERT_SCENE_TYPE(scene);
    LabelSceneData* sceneData = scene->sceneData;

    size_t textLength = strlen(text);
    if (textLength + 1 > sceneData->bufferSize)
    {
        sceneData->bufferSize = textLength + 1;
        sceneData->buffer = realloc(sceneData->buffer, sceneData->bufferSize);
    }

    strcpy(sceneData->buffer, text);

    scene->transform.size = Raylib_MeasureText(text, sceneData->fontSize);
}
