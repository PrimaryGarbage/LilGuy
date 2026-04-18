#include "main_char_foot_scene.h"
#include "graphics/color.h"
#include "graphics/graphics.h"
#include "scene.h"
#include "scene_type.h"
#include "physics/transform.h"
#include "vector2.h"
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

typedef struct MainCharFootSceneData {
    bool inAnimation;
    Vector2 animationTargetPoint;
    Vector2 animationInitialPoint;
    double animationElapsed;
} MainCharFootSceneData;

const float c_footSize = 4.0f;
const double c_animationLength = 0.07;
const float c_animationFootYMaxOffset = 10.0f;

static void Update(Scene* scene, double deltatime)
{
    ASSERT_SCENE_TYPE(scene, SCENE_TYPE_MAIN_CHAR_FOOT);

    MainCharFootSceneData* sceneData = scene->sceneData;

    if (sceneData->inAnimation)
    {
        sceneData->animationElapsed += deltatime;

        if (sceneData->animationElapsed < c_animationLength)
        {
            float weightX = sceneData->animationElapsed / c_animationLength;
            float weightY = weightX < 0.5f ? weightX * 2.0f : 2.0f - weightX * 2.0f;
            scene->transform.position = Vector2_Lerp(sceneData->animationInitialPoint, sceneData->animationTargetPoint, weightX);
            scene->transform.position.y += weightY * c_animationFootYMaxOffset;
        }
        else
        {
            sceneData->inAnimation = false;
        }
    }
}

static void Draw(Scene* scene)
{
    constexpr Color color = (Color) { .r = 150, .g = 150, .b = 120, .a = 255 };

    ASSERT_SCENE_TYPE(scene, SCENE_TYPE_MAIN_CHAR_FOOT);

    Graphics_SetTransformW(&scene->globalTransform);
    Graphics_DrawCircleT(c_footSize, color, DRAW_ORDER_MAIN_CHAR);
    Graphics_ClearTransform();

    // origin
    //Graphics_DrawSquareW(scene->globalTransform.position, 1.0f, COLOR_WHITE, DRAW_ORDER_TOP);
}

Scene* MainCharFootScene_Create(Scene* parent)
{
    Scene* scene = malloc(sizeof(Scene));

    MainCharFootSceneData* sceneData = malloc(sizeof(MainCharFootSceneData));
    sceneData->inAnimation = false;
    sceneData->animationElapsed = 0.0;
    sceneData->animationInitialPoint = Vector2_Zero();
    sceneData->animationTargetPoint = Vector2_Zero();

    scene->sceneData = sceneData;
    scene->type = SCENE_TYPE_MAIN_CHAR_FOOT;
    scene->transform.position = Vector2_Zero();
    scene->transform.scale = Vector2_New(1.0f, 1.0f);
    scene->transform.origin = Vector2_New(c_footSize * 0.5f, 0.0f);
    scene->transform.rotation = 0.0f;
    scene->transform.topLevel = false;
    scene->childrenCount = 0u;
    scene->name = "Main Char Foot Scene";
    scene->parent = parent;
    
    scene->startFunction = NULL;
    scene->updateFunction = Update;
    scene->drawFunction = Draw;
    scene->cleanupFunction = NULL;

    return scene;
}

void MainCharFootScene_MoveFoot(Scene* scene, Vector2 point)
{
    ASSERT_SCENE_TYPE(scene, SCENE_TYPE_MAIN_CHAR_FOOT);

    MainCharFootSceneData* sceneData = scene->sceneData;

    if (sceneData->inAnimation) return;

    sceneData->animationInitialPoint = scene->transform.position;
    sceneData->animationTargetPoint = point;
    sceneData->animationElapsed = 0.0;
    sceneData->inAnimation = true;
}

bool MainCharFootScene_InAnimation(Scene* scene)
{
    ASSERT_SCENE_TYPE(scene, SCENE_TYPE_MAIN_CHAR_FOOT);
    MainCharFootSceneData* sceneData = scene->sceneData;
    return sceneData->inAnimation;
}
