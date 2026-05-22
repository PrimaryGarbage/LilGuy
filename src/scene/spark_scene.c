#include "spark_scene.h"
#include "animated_sprite_scene.h"
#include "graphics/graphics.h"
#include "random.h"
#include "scene.h"
#include "scene/point_light_scene.h"
#include "sprite_scene.h"
#include "scene_type.h"
#include "tween.h"
#include "vector2.h"

#include <stdlib.h>

#define SCENE_TYPE SCENE_TYPE_SPARK

typedef struct SparkSceneStaticData {
    bool initialized;
    Texture2D sparkTexture;
} SparkSceneStaticData;

SparkSceneStaticData s_staticData;

constexpr double c_sparkLifetime = 0.05f;

static void OnFinishCallback(Scene* scene)
{
    Scene_QueueFree(scene);
}

Scene* SparkScene_Create(Scene* parent, Vector2 position, float scale, Color color, bool lightOn, Color lightColor, const char* name)
{
    if (!s_staticData.initialized)
    {
        const char* sparkImagePath = "res/images/ProjectileSpark.png";
        s_staticData.sparkTexture = Graphics_LoadTexture(sparkImagePath);
        if (s_staticData.sparkTexture.id == 0)
            PANIC_EX(LogErrorM("Failed to load texture. Path: %s", sparkImagePath););

        s_staticData.initialized = true;
    }

    Scene* scene = malloc(sizeof(Scene));
    Scene_DefaultInit(scene, SCENE_TYPE_SPARK, parent, name);

    Scene* sprite = SpriteScene_CreateWithTexture(scene, &s_staticData.sparkTexture, "Spark Sprite");
    SpriteScene_SetTint(sprite, color);
    sprite->transform.position = position;
    sprite->transform.rotation = RandomFloat() * 360.0f;
    sprite->transform.origin = (Vector2){ .x = s_staticData.sparkTexture.width * 0.5f, .y = s_staticData.sparkTexture.height * 0.5f };
    sprite->transform.scale = Vector2_Uniform(scale * (RandomFloat() + 0.5f));

    if (lightOn)
    {
        float sparkLightRadius = RandomFloat() * 50.0f * scale + 20.0f * scale;
        PointLightScene_Create(sprite, "Spark Light", lightColor, sparkLightRadius);
    }

    Tween_CreateTimer(c_sparkLifetime, scene, OnFinishCallback);

    return scene;
}