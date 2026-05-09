#include "point_light_scene.h"
#include "graphics/graphics.h"
#include "scene/scene.h"
#include "scene_type.h"
#include <stdlib.h>

#define SCENE_TYPE SCENE_TYPE_POINT_LIGHT

typedef struct PointLightSceneData {
    Color lightColor;
    float lightRadius;
} PointLightSceneData;

static void Draw(Scene* scene)
{
    PointLightSceneData* sceneData = scene->sceneData;

    Graphics_DrawLight(scene->globalTransform.position, sceneData->lightRadius, sceneData->lightColor);
}

Scene* PointLightScene_Create(Scene* parent, const char* name, Color lightColor, float lightRadius)
{
    Scene* scene = malloc(sizeof(Scene));
    Scene_DefaultInit(scene, SCENE_TYPE_POINT_LIGHT, parent, name);
    PointLightSceneData* sceneData = malloc(sizeof(PointLightSceneData));
    scene->sceneData = sceneData;
    sceneData->lightColor = lightColor;
    sceneData->lightRadius = lightRadius;

    scene->drawFunction = Draw;

    return scene;
}

void PointLightScene_SetLight(Scene* scene, Color color, float radius)
{
    ASSERT_SCENE_TYPE(scene);

    PointLightSceneData* sceneData = scene->sceneData;
    sceneData->lightColor = color;
    sceneData->lightRadius = radius;
}