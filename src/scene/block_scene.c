#include "block_scene.h"
#include <stdlib.h>

Scene* BlockScene_Create(Scene* parent, Rect rect, Color color)
{
    Scene* scene = malloc(sizeof(Scene));

    scene->type = SCENE_TYPE_BLOCK;
    scene->transform.position = Vector2_Zero();
    scene->transform.scale = Vector2_Uniform(1.0f);
    scene->transform.origin = Vector2_Zero();
    scene->transform.rotation = 0.0f;
    scene->transform.topLevel = false;
    scene->childrenCount = 0u;
    scene->parent = NULL;

    Scene_UpdateGlobalTransform(scene, false);

    scene->startFunction = NULL;
    scene->updateFunction = NULL;
    scene->drawFunction = NULL;

    return scene;
}
