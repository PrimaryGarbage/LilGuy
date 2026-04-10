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

static const Transform s_defaultTransform = {
    .position = (Vector2){ 0.0f, 0.0f },
    .rotation = 0.0f,
    .scale = (Vector2){ 1.0f, 1.0f },
    .origin = (Vector2){ 0.0f, 0.0f }
};

static const Transform* s_transform = &s_defaultTransform;

Result Graphics_CaptureScreen(Image* image_out)
{
    return CaptureScreen(image_out);
}

float Graphics_GetScreenHeight()
{
    return Raylib_GetScreenHeight();
}

float Graphics_GetScreenWidth()
{
    return Raylib_GetScreenWidth();
}

Vector2 Graphics_GetScreenSize()
{
    return (Vector2){ Raylib_GetScreenWidth(), Raylib_GetScreenHeight() };
}

Rect Graphics_GetScreenRect()
{
    return (Rect) {
        .x = 0.0f, 
        .y = 0.0f,
        .width = Raylib_GetScreenWidth(), 
        .height = Raylib_GetScreenHeight() 
    };
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
        Vector2_Mult(s_transform->origin, s_transform->scale), s_transform->rotation, color);
}

void Graphics_DrawRectT(Vector2 size, Color color)
{
    Raylib_DrawRectanglePro((Rect){ s_transform->position.x, s_transform->position.y, size.x * s_transform->scale.x, size.y * s_transform->scale.y}, 
        Vector2_Mult(s_transform->origin, s_transform->scale), s_transform->rotation, color);
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
    Raylib_DrawTexturePro(*texture, (Rect){ 0.0f, 0.0f, texture->width, texture->height }, Graphics_GetScreenRect(), (Vector2){ 0.0f, 0.0f }, 0.0f, COLOR_WHITE);
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