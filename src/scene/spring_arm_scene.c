#include "spring_arm_scene.h"
#include <stdlib.h>

#define SCENE_TYPE SCENE_TYPE_SPRING_ARM

typedef struct SpringArmSceneData {
    Scene* target;
    Vector2 initialOffset;
    float stiffness;
} SpringArmSceneData;

static void Update(Scene* scene, double _)
{
    SpringArmSceneData* sceneData = scene->sceneData;

    Vector2 targetPosition = Vector2_Add(sceneData->target->globalTransform.position, sceneData->initialOffset);
    Vector2 newPosition = Vector2_Lerp(scene->transform.position, targetPosition, sceneData->stiffness);
    scene->transform.position = newPosition;
}

Scene* SpringArmScene_Create(Scene* parent, Scene* target, Vector2 initialOffset, float stiffness, const char* name)
{
    Scene* scene = malloc(sizeof(Scene));
    Scene_DefaultInit(scene, SCENE_TYPE, parent, name);
    SpringArmSceneData* sceneData = malloc(sizeof(SpringArmSceneData));
    scene->sceneData = sceneData;
    sceneData->target = target;
    sceneData->initialOffset = initialOffset,
    sceneData->stiffness = Clampf(0.0f, 1.0f, stiffness);

    scene->transform.topLevel = true;

    scene->updateFunction = Update;

    return scene;
}
