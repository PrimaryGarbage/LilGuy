#include "empty_scene.h"
#include <stdlib.h>

Scene* EmptyScene_Create()
{
    Scene* scene = malloc(sizeof(Scene));

    Scene_DefaultInit(scene, SCENE_TYPE_EMPTY, "Empty Scene");

    return scene;
}