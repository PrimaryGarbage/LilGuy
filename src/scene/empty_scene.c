#include "empty_scene.h"
#include <stdlib.h>

#define SCENE_TYPE SCENE_TYPE_EMPTY

Scene* EmptyScene_Create(Scene* parent, const char* name)
{
    Scene* scene = malloc(sizeof(Scene));
    Scene_DefaultInit(scene, SCENE_TYPE_EMPTY, parent, name);

    return scene;
}