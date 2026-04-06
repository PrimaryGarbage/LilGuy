
#define Vector2 Rl_Vector2
#define Color Rl_Color
#define Image Rl_Image
#define PixelFormat Rl_PixelFormat
#define Texture2D Rl_Texture2D

#include "raylib.h"
#include "rlgl.h"

#undef Vector2
#undef Color
#undef Image
#undef PixelFormat
#undef Texture2D

#include <string.h>

#define TYPE_PUN(typeFrom, typeTo, valueFrom, valueTo) \
do { \
    static_assert(sizeof(typeFrom) == sizeof(typeTo), "Struct sizes mismatch!"); \
    static_assert(_Alignof(typeFrom) == _Alignof(typeTo), "Struct alignments mismatch!"); \
    memcpy(&valueTo, &valueFrom, sizeof(typeTo)); \
} while(0)

#include "raylib_wrapper.h"

static inline Rl_Color ColorToRlColor(Color color)
{
    Rl_Color out;
    TYPE_PUN(Color, Rl_Color, color, out);
    return out;
}

static inline Color RlColorToColor(Rl_Color color)
{
    Color out;
    TYPE_PUN(Rl_Color, Color, color, out);
    return out;
}

static inline Rl_Vector2 Vector2ToRlVector2(Vector2 vec)
{
    Rl_Vector2 out;
    TYPE_PUN(Vector2, Rl_Vector2, vec, out);
    return out;
}

static inline Vector2 RlVector2ToVector2(Rl_Vector2 vec)
{
    Vector2 out;
    TYPE_PUN(Rl_Vector2, Vector2, vec, out);
    return out;
}

static inline Rectangle RectToRlRect(Rect rect)
{
    Rectangle out;
    TYPE_PUN(Rect, Rectangle, rect, out);
    return out;
}

static inline Rect RlRectToRect(Rectangle rect)
{
    Rect out;
    TYPE_PUN(Rectangle, Rect, rect, out);
    return out;
}

static inline Image RlImageToImage(Rl_Image image)
{
    return (Image) {
        .data = image.data,
        .format = image.format,
        .width = image.width,
        .height = image.height,
        .dataSize = image.width * image.height * 4
    };
}

static inline Rl_Image ImageToRlImage(Image image)
{
    return (Rl_Image) {
        .data = image.data,
        .format = image.format,
        .width = image.width,
        .height = image.height,
        .mipmaps = 1
    };
}

static inline Texture2D RlTextureToTexture(Rl_Texture2D texture)
{
    return (Texture2D) {
        .id = texture.id,
        .format = texture.format,
        .width = texture.width,
        .height = texture.height
    };
}

static inline Rl_Texture2D TextureToRlTexture(Texture2D texture)
{
    return (Rl_Texture2D) {
        .id = texture.id,
        .format = texture.format,
        .width = texture.width,
        .height = texture.height,
        .mipmaps = 1
    };
}

void Raylib_InitWindow(u32 windowWidth, u32 windowHeight, const char* windowTitle)
{
    InitWindow(windowWidth, windowHeight, windowTitle);
    SetWindowState(FLAG_VSYNC_HINT);
    ToggleBorderlessWindowed();
}

bool Raylib_WindowShouldClose()
{
    return WindowShouldClose();
}

void Raylib_CloseWindow()
{
    CloseWindow();
}

void Raylib_PollInputEvents()
{
    PollInputEvents();
}

void Raylib_BeginDrawing()
{
    BeginDrawing();
}

void Raylib_EndDrawing()
{
    EndDrawing();
}

void Raylib_SwapBuffers()
{
    SwapScreenBuffer();
}

u32 Raylib_GetScreenWidth()
{
    return GetScreenWidth();
}

u32 Raylib_GetScreenHeight()
{
    return GetScreenHeight();
}

void Raylib_ClearBackground(Color color)
{
    ClearBackground(ColorToRlColor(color));
}

bool Raylib_IsKeyPressed(int key)
{
    return IsKeyPressed(key);
}

bool Raylib_IsKeyReleased(int key)
{
    return IsKeyReleased(key);
}

bool Raylib_IsKeyDown(int key)
{
    return IsKeyDown(key);
}

bool Raylib_IsMouseButtonPressed(int button)
{
    return IsMouseButtonPressed(button);
}

bool Raylib_IsMouseButtonReleased(int button)
{
    return Raylib_IsMouseButtonReleased(button);
}

bool Raylib_IsMouseButtonDown(int button)
{
    return IsMouseButtonDown(button);
}

Vector2 Raylib_GetMousePosition()
{
    return RlVector2ToVector2(GetMousePosition());
}

Image Raylib_LoadImageFromScreen()
{
    return RlImageToImage(LoadImageFromScreen());
}

Texture2D Raylib_LoadTextureFromImage(const Image* image)
{
    return RlTextureToTexture(LoadTextureFromImage(ImageToRlImage(*image)));
}

void Raylib_UnloadTexture(Texture2D texture)
{
    UnloadTexture(TextureToRlTexture(texture));
}

void Raylib_rlDrawRenderBatchActive()
{
    rlDrawRenderBatchActive();
}

float Raylib_GetFrameTime()
{
    return GetFrameTime();
}

void Raylib_HideWindow()
{
    SetWindowState(FLAG_WINDOW_HIDDEN);
}

void Raylib_ShowWindow()
{
    ClearWindowState(FLAG_WINDOW_HIDDEN);
}

void Raylib_DrawTexturePro(Texture2D texture, Rect source, Rect dest, Vector2 origin, float rotation, Color tint)
{
    DrawTexturePro(TextureToRlTexture(texture), RectToRlRect(source), RectToRlRect(dest), Vector2ToRlVector2(origin), rotation, ColorToRlColor(tint));
}

void Raylib_DrawRectanglePro(Rect rect, Vector2 origin, float rotation, Color color)
{
    DrawRectanglePro(RectToRlRect(rect), Vector2ToRlVector2(origin), rotation, ColorToRlColor(color));
}

void Raylib_DrawLineEx(Vector2 startPos, Vector2 endPos, float thick, Color color)
{
    DrawLineEx(Vector2ToRlVector2(startPos), Vector2ToRlVector2(endPos), thick, ColorToRlColor(color));
}

void Raylib_DrawCircleV(Vector2 position, float radius, Color color)
{
    DrawCircleV(Vector2ToRlVector2(position), radius, ColorToRlColor(color));
}
