#include "graphics.h"
#include "graphics/color.h"
#include "graphics/image.h"
#include "result.h"
#include "scene/transform.h"
#include "stb_image_write.h"
#include <assert.h>
#include <math.h>
#include <stddef.h>
#include "raylib_wrapper.h"
#include "screen_capture.h"
#include "vector2.h"
#include "window/window.h"

static const Transform s_defaultTransform = {
    (Vector2){ 0.0f, 0.0f },
    0.0f,
    (Vector2){ 1.0f, 1.0f },
    (Vector2){ 0.0f, 0.0f }
};

static const Transform* s_transform = &s_defaultTransform;

Result Graphics_CaptureScreen(Image* image_out)
{
    return CaptureScreen(image_out);
}

void Graphics_SetTransform(const Transform* transform)
{
    s_transform = transform;
}

void Graphics_ClearTransform()
{
    s_transform = &s_defaultTransform;
}

void Graphics_DrawRect(Rect rect, Color color)
{
    Raylib_DrawRectanglePro((Rect){ rect.x, rect.y, rect.width * s_transform->scale.x, rect.height * s_transform->scale.y}, 
        Vector2_Mult(s_transform->origin, s_transform->scale), 0.0f, color);
}

void Graphics_DrawSquare(Vector2 position, float size, Color color)
{
    Graphics_DrawRect((Rect){
         .x = position.x,
         .y = position.y,
         .width = size,
         .height = size
    }, 
    color);
}

void Graphics_DrawCircle(Vector2 position, float radius, Color color)
{
    Raylib_DrawCircleV(position, radius, color);
}

void Graphics_DrawLine(Vector2 a, Vector2 b, float width, Color color)
{
    Raylib_DrawLineEx(a, b, width, color);
}

Texture2D Graphics_LoadTextureFromImage(const Image* image)
{
    return Raylib_LoadTextureFromImage(image);
}

void Graphics_UnloadTexture(Texture2D texture)
{
    Raylib_UnloadTexture(texture);
}

void Graphics_DrawTexture(const Texture2D* texture, Rect dest)
{
    Raylib_DrawTexturePro(*texture, (Rect){ 0.0f, 0.0f, texture->width, texture->height }, dest, (Vector2){ 0.0f, 0.0f }, 0.0f, COLOR_WHITE);
}

void Graphics_DrawTextureFullscreen(const Texture2D* texture)
{
    Raylib_DrawTexturePro(*texture, (Rect){ 0.0f, 0.0f, texture->width, texture->height }, Window_GetWindowRect(), (Vector2){ 0.0f, 0.0f }, 0.0f, COLOR_WHITE);
}

void Graphics_ClearBackground(Color color)
{
    Raylib_ClearBackground(color);
}

void Graphics_Flush()
{
    Raylib_rlDrawRenderBatchActive();
    Raylib_SwapBuffers();
}