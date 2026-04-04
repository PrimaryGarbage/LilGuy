#ifndef __SCENE_H__
#define __SCENE_H__

#include "short_types.h"
#include "transform.h"

#define SCENE_DRAW(scene) do { scene->drawFunction(scene); } while(0)
#define SCENE_UPDATE(scene, deltatime) do { scene->updateFunction(scene, deltatime); } while(0)

#define SCENE_CHILDREN_MAX 128

typedef struct Scene Scene;

typedef void(*SceneDrawFunction)(Scene* scene);
typedef void(*SceneUpdateFunction)(Scene* scene, double deltatime);

typedef struct Scene {
    struct Scene* parent;
    struct Scene* children[SCENE_CHILDREN_MAX];
    u32 childrenCount;
    Transform transform;
    SceneUpdateFunction updateFunction;
    SceneDrawFunction drawFunction;
    void* customData;
} Scene;

void Scene_Destroy(Scene* scene);
void Scene_AddChild(Scene* scene, Scene* child);

#endif // __SCENE_H__