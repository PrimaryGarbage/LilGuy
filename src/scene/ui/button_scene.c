#include "button_scene.h"
#include "graphics/draw_order.h"
#include "graphics/graphics.h"
#include "input/input.h"
#include "input/input_button.h"
#include "scene/scene.h"
#include <stdlib.h>

#define SCENE_TYPE SCENE_TYPE_BUTTON

typedef struct {
    Color buttonColor;
    const char* text;
    ButtonScene_OnPressedCallback callback;
    Scene* callbackOwner;
    float borderThickness;
    Color borderColor;
    i32 tag;
} ButtonSceneData;

constexpr u32 c_padding = 5u;
constexpr u32 c_fontSize = 14;

static void Update(Scene* scene, double _)
{
    if (!scene->visible) return;

    ButtonSceneData* sceneData = scene->sceneData;
    const Vector2* size = &scene->transform.size;
    const Vector2* position = &scene->globalTransform.position;

    Vector2 mousePos = Input_GetMousePosition();
    if (Input_IsMouseButtonJustPressed(INPUT_MOUSE_BUTTON_LEFT) && mousePos.x > position->x && mousePos.x < (position->x + size->x) &&
        mousePos.y > position->y && mousePos.y < (position->y + size->y))
    {
        sceneData->callback(sceneData->callbackOwner, sceneData->tag);
    }
}

static void Draw(Scene* scene)
{
    ButtonSceneData* sceneData = scene->sceneData;

    if (sceneData->borderThickness > 0.0f)
    {
        Graphics_DrawRect((Rect) {
            .x = scene->globalTransform.position.x - sceneData->borderThickness,
            .y = scene->globalTransform.position.y - sceneData->borderThickness,
            .width = scene->transform.size.x + sceneData->borderThickness * 2.0f,
            .height = scene->transform.size.y + sceneData->borderThickness * 2.0f
        }, sceneData->borderColor, DRAW_ORDER_UI);
    }

    Graphics_DrawRect((Rect) {
        .x = scene->globalTransform.position.x,
        .y = scene->globalTransform.position.y,
        .width = scene->transform.size.x,
        .height = scene->transform.size.y
    }, sceneData->buttonColor, DRAW_ORDER_UI);

    Graphics_DrawText(sceneData->text, Vector2_AddScalar(scene->globalTransform.position, c_padding), c_fontSize, COLOR_WHITE, DRAW_ORDER_UI);
}

Scene* ButtonScene_Create(Scene* parent, const char* name, Vector2 size, Color color, const char* text, i32 tag, ButtonScene_OnPressedCallback callback, Scene* callbackOwner)
{
    Scene* scene = malloc(sizeof(Scene));
    Scene_DefaultInit(scene, SCENE_TYPE_BUTTON, parent, name);
    ButtonSceneData* sceneData = malloc(sizeof(ButtonSceneData));
    scene->sceneData = sceneData;

    scene->transform.size = size;

    sceneData->buttonColor = color;
    sceneData->text = text;
    sceneData->callback = callback;
    sceneData->callbackOwner = callbackOwner;
    sceneData->borderThickness = 0.0f;
    sceneData->borderColor = COLOR_BLACK;
    sceneData->tag = tag;

    scene->updateFunction = Update;
    scene->drawFunction = Draw;

    return scene;
}

void ButtonScene_SetBorder(Scene* scene, float thickness, Color color)
{
    ASSERT_SCENE_TYPE(scene);
    ButtonSceneData* sceneData = scene->sceneData;
    sceneData->borderThickness = thickness;
    sceneData->borderColor = color;
}