#include "main_char_hand_scene.h"
#include "graphics/color.h"
#include "graphics/graphics.h"
#include "scene/scene.h"
#include "scene_type.h"
#include <stdlib.h>

constexpr Vector2 c_handSize = { 5.0f, 10.0f };

static void Draw(Scene* scene)
{
    Graphics_SetTransform(&scene->globalTransform);
    Graphics_DrawRectT(c_handSize, COLOR_GREEN);
    Graphics_ClearTransform();
}

Scene* MainCharHandScene_Create(Scene* parent)
{
    Scene* scene = malloc(sizeof(Scene));
    Scene_DefaultInit(scene, SCENE_TYPE_MAIN_CHAR_HAND, "Main Char Hand Scene");

    scene->parent = parent;
    scene->transform.origin = Vector2_MultScalar(c_handSize, 0.5f);

    scene->drawFunction = Draw;

    return scene;
}