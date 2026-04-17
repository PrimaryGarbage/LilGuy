#ifndef __SCENE_H__
#define __SCENE_H__

#include "short_types.h"
#include "physics/transform.h"
#include "scene_type.h"
#include <assert.h>

#define ASSERT_SCENE_TYPE(scene, sceenType) do { assert(scene->type == sceenType && "Invalid scene type"); } while(0)

#define SCENE_CHILDREN_MAX 128

typedef struct Scene Scene;

typedef void(*SceneStartFunction)(Scene* scene);
typedef void(*SceneUpdateFunction)(Scene* scene, double deltatime);
typedef void(*SceneDrawFunction)(Scene* scene);
typedef void(*SceneCleanupFunction)(Scene* scene);

typedef struct Scene {
    SceneType type;
    struct Scene* parent;
    struct Scene* children[SCENE_CHILDREN_MAX];
    u32 childrenCount;
    Transform transform;
    Transform globalTransform;
    SceneStartFunction startFunction;
    SceneUpdateFunction updateFunction;
    SceneDrawFunction drawFunction;
    SceneCleanupFunction cleanupFunction;
    bool visible;
    const char* name;
    void* sceneData;
} Scene;

void Scene_Free(Scene* scene);
void Scene_AddChild(Scene* scene, Scene* child);
void Scene_UpdateGlobalTransform(Scene* scene, bool recurse);
void Scene_DefaultInit(Scene* scene, SceneType type, const char* name);

void Scene_Start(Scene* scene);
void Scene_Update(Scene* scene, double deltatime);
void Scene_Draw(Scene* scene);
void Scene_Cleanup(Scene* scene);

#endif // __SCENE_H__