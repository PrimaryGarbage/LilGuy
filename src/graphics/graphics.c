#include "graphics.h"
#include "graphics/color.h"
#include "graphics/image.h"
#include "graphics_transform.h"
#include "rect.h"
#include "result.h"
#include "physics/transform.h"
#include "stb_image_write.h"
#include <assert.h>
#include <basetsd.h>
#include <math.h>
#include <stddef.h>
#include "raylib_wrapper.h"
#include "screen_capture.h"
#include "vector2.h"

constexpr GraphicsTransform c_defaultTransform = {
    .position = (Vector2){ 0.0f, 0.0f },
    .rotation = 0.0f,
    .scale = (Vector2){ 1.0f, 1.0f },
    .origin = (Vector2){ 0.0f, 0.0f },
};

static GraphicsTransform s_transform = c_defaultTransform;

static Vector2 WorldToScreenOrigin(Vector2 worldOrigin, Vector2 size)
{
    return (Vector2){ .x = worldOrigin.x, .y = size.y - worldOrigin.y };
}

Result Graphics_CaptureScreen(Image* image_out)
{
    return CaptureScreen(image_out);
}

Vector2 Graphics_WorldToScreen(Vector2 vec)
{
    return (Vector2){ vec.x, Graphics_GetScreenHeight() - vec.y };
}

Vector2 Graphics_ScreenToWorld(Vector2 vec)
{
    return (Vector2){ vec.x, Graphics_GetScreenHeight() - vec.y };
}

Rect Graphics_WorldToScreenRect(Rect rect)
{
    return (Rect) {
        .x = rect.x,
        .y = Graphics_GetScreenHeight()- rect.y - rect.height,
        .width = rect.width,
        .height = rect.height
    };
}

Rect Graphics_ScreenToWorldRect(Rect rect)
{
    return (Rect) {
        .x = rect.x,
        .y = Graphics_GetScreenHeight() - rect.y - rect.height,
        .width = rect.width,
        .height = rect.height
    };
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

void Graphics_SetTransform(GraphicsTransform transform)
{
    s_transform = transform;
}

void Graphics_SetTransformW(const Transform* transform)
{
    Graphics_SetTransform((GraphicsTransform) {
        .position = Graphics_WorldToScreen(transform->position),
        .rotation = transform->rotation,
        .scale = transform->scale,
        .origin = (Vector2){ .x = transform->origin.x, .y = transform->origin.y},
    });
}

void Graphics_ClearTransform()
{
    s_transform = c_defaultTransform;
}

void Graphics_DrawRect(Rect rect, Color color)
{
    Raylib_DrawRectanglePro(rect, Vector2_Zero(), 0.0f, color);
}

void Graphics_DrawRectT(Vector2 size, Color color)
{
    Vector2 origin = WorldToScreenOrigin(s_transform.origin, size);

    Raylib_DrawRectanglePro((Rect){ s_transform.position.x, s_transform.position.y, size.x * s_transform.scale.x, size.y * s_transform.scale.y}, 
        Vector2_Mult(origin, s_transform.scale), -s_transform.rotation, color);
} 

void Graphics_DrawRectW(Rect rect, Color color)
{
    Raylib_DrawRectanglePro(Graphics_WorldToScreenRect(rect), Vector2_Zero(), 0.0f, color);
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

void Graphics_DrawSquareW(Vector2 position, float size, Color color)
{
    Graphics_DrawRect(Rect_FromVectors(Graphics_WorldToScreen(position), Vector2_Uniform(size)), color);
}

void Graphics_DrawCircle(Vector2 position, float radius, Color color)
{
    Raylib_DrawCircleV(position, radius, color);
}

void Graphics_DrawCircleT(float radius, Color color)
{
    Raylib_DrawCircleV(s_transform.position, radius * (s_transform.scale.x + s_transform.scale.y) * 0.5f, color);
}

void Graphics_DrawCircleW(Vector2 position, float radius, Color color)
{
    Raylib_DrawCircleV(Graphics_WorldToScreen(position), radius, color);
}

void Graphics_DrawLineSimple(Vector2 a, Vector2 b, Color color)
{
    Raylib_DrawLineV(a, b, color);
}

void Graphics_DrawLineSimpleW(Vector2 a, Vector2 b, Color color)
{
    Raylib_DrawLineV(Graphics_WorldToScreen(a), Graphics_WorldToScreen(b), color);
}

void Graphics_DrawLine(Vector2 a, Vector2 b, float width, Color color)
{
    Raylib_DrawLineEx(a, b, width, color);
}

void Graphics_DrawLineW(Vector2 a, Vector2 b, float width, Color color)
{
    Raylib_DrawLineEx(Graphics_WorldToScreen(a), Graphics_WorldToScreen(b), width, color);
}

void Graphics_DrawVector(Vector2 a, Vector2 b, Color color)
{
    constexpr float arrowSize = 15.0f;
    constexpr float arrowAngle = 20.0f;
    Vector2 arrowVec = Vector2_Normalize(Vector2_Sub(a, b));

    Raylib_DrawLineV(a, b, color);
    Raylib_DrawLineV(b, Vector2_Add(b, Vector2_MultScalar(Vector2_Rotate(arrowVec, arrowAngle), arrowSize)), color);
    Raylib_DrawLineV(b, Vector2_Add(b, Vector2_MultScalar(Vector2_Rotate(arrowVec, -arrowAngle), arrowSize)), color);
}

void Graphics_DrawVectorFromPoint(Vector2 point, Vector2 vec, Color color)
{
    constexpr float arrowSize = 15.0f;
    constexpr float arrowAngle = 20.0f;
    Vector2 sum = Vector2_Add(point, vec);
    Vector2 arrowVec = Vector2_Normalize(Vector2_MultScalar(vec, -1.0f));

    Raylib_DrawLineV(point, sum, color);
    Raylib_DrawLineV(sum, Vector2_Add(sum, Vector2_MultScalar(Vector2_Rotate(arrowVec, arrowAngle), arrowSize)), color);
    Raylib_DrawLineV(sum, Vector2_Add(sum, Vector2_MultScalar(Vector2_Rotate(arrowVec, -arrowAngle), arrowSize)), color);
}

void Graphics_DrawVectorW(Vector2 a, Vector2 b, Color color)
{
    a = Graphics_WorldToScreen(a);
    b = Graphics_WorldToScreen(b);

    constexpr float arrowSize = 15.0f;
    constexpr float arrowAngle = 20.0f;
    Vector2 arrowVec = Vector2_Normalize(Vector2_Sub(a, b));

    Raylib_DrawLineV(a, b, color);
    Raylib_DrawLineV(b, Vector2_Add(b, Vector2_MultScalar(Vector2_Rotate(arrowVec, arrowAngle), arrowSize)), color);
    Raylib_DrawLineV(b, Vector2_Add(b, Vector2_MultScalar(Vector2_Rotate(arrowVec, -arrowAngle), arrowSize)), color);
}

void Graphics_DrawVectorFromPointW(Vector2 point, Vector2 vec, Color color)
{
    point = Graphics_WorldToScreen(point);
    vec.y = -vec.y;

    constexpr float arrowSize = 15.0f;
    constexpr float arrowAngle = 20.0f;
    Vector2 sum = Vector2_Add(point, vec);
    Vector2 arrowVec = Vector2_Normalize(Vector2_MultScalar(vec, -1.0f));

    Raylib_DrawLineV(point, sum, color);
    Raylib_DrawLineV(sum, Vector2_Add(sum, Vector2_MultScalar(Vector2_Rotate(arrowVec, arrowAngle), arrowSize)), color);
    Raylib_DrawLineV(sum, Vector2_Add(sum, Vector2_MultScalar(Vector2_Rotate(arrowVec, -arrowAngle), arrowSize)), color);
}

Texture2D Graphics_LoadTextureFromImage(const Image* image)
{
    return Raylib_LoadTextureFromImage(image);
}

Texture2D Graphics_LoadTexture(const char* imagePath)
{
    return Raylib_LoadTexture(imagePath);
}

void Graphics_UnloadTexture(Texture2D texture)
{
    Raylib_UnloadTexture(texture);
}

void Graphics_DrawTexture(const Texture2D* texture, Rect dest)
{
    Raylib_DrawTexturePro(*texture, (Rect){ 0.0f, 0.0f, texture->width, texture->height }, dest, (Vector2){ 0.0f, 0.0f }, 0.0f, COLOR_WHITE);
}

void Graphics_DrawTextureT(const Texture2D* texture)
{
    Rect src = (Rect){ 0.0f, 0.0f, texture->width, texture->height };
    Rect dest = (Rect){ s_transform.position.x, s_transform.position.y, texture->width, texture->height };

    Raylib_DrawTexturePro(*texture, src, dest, s_transform.origin, -s_transform.rotation, COLOR_WHITE);
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

Vector2 Graphics_GetMousePosition()
{
    return Raylib_GetMousePosition();
}

Vector2 Graphics_GetMousePositionW()
{
    return Graphics_ScreenToWorld(Raylib_GetMousePosition());
}