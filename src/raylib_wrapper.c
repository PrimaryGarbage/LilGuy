#include "graphics/image.h"
#include "logging.h"
#include "result.h"
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

#include "raylib_wrapper.h"

static inline Rl_Color ColorToRlColor(Color color)
{
    return (Rl_Color){
        .r = color.r * 255,
        .g = color.g * 255,
        .b = color.b * 255,
        .a = color.a * 255
    };
}

static inline Color RlColorToColor(Rl_Color color)
{
    return (Color){
        .r = color.r / 255.0f,
        .g = color.g / 255.0f,
        .b = color.b / 255.0f,
        .a = color.a / 255.0f
    };
}

static inline Rl_Vector2 Vector2ToRlVector2(Vector2 vec)
{
    return (Rl_Vector2){ vec.x, vec.y };
}

static inline Vector2 RlVector2ToVector2(Rl_Vector2 vec)
{
    return (Vector2){ vec.x, vec.y };
}

static inline Rectangle RectToRlRect(Rect rect)
{
    return (Rectangle){
        .x = rect.a.x,
        .y = rect.a.y,
        .width = rect.b.x - rect.a.x,
        .height = rect.b.y - rect.a.y
    };
}

static inline Rect RlRectToRect(Rectangle rect)
{
    return (Rect){
        .a = { rect.x, rect.y },
        .b = { rect.x + rect.width, rect.y + rect.height }
    };
}

static PixelFormat RlPixelFormatToPixelFormat(Rl_PixelFormat format)
{
    switch(format)
    {
        case PIXELFORMAT_UNCOMPRESSED_R8G8B8A8:
            return PIXEL_FORMAT_RGBA;
            break;
        default:
            PANIC_EX(LogErrorM("Converting image with unsupported pixel format"););
            break;
    }
}

static Rl_PixelFormat PixelFormatToRlPixelFormat(PixelFormat format)
{
    switch(format)
    {
        case PIXEL_FORMAT_RGBA:
            return PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
            break;
        default:
            PANIC_EX(LogErrorM("Converting image with unsupported pixel format"););
            break;
    }
}

static inline Image RlImageToImage(Rl_Image image)
{
    return (Image) {
        .data = image.data,
        .format = RlPixelFormatToPixelFormat(image.format),
        .width = image.width,
        .height = image.height,
        .dataSize = image.width * image.height * 4
    };
}

static inline Rl_Image ImageToRlImage(Image image)
{
    return (Rl_Image) {
        .data = image.data,
        .format = PixelFormatToRlPixelFormat(image.format),
        .width = image.width,
        .height = image.height,
        .mipmaps = 1
    };
}

static inline Texture2D RlTextureToTexture(Rl_Texture2D texture)
{
    return (Texture2D) {
        .id = texture.id,
        .format = RlPixelFormatToPixelFormat(texture.format),
        .width = texture.width,
        .height = texture.height
    };
}

static inline Rl_Texture2D TextureToRlTexture(Texture2D texture)
{
    return (Rl_Texture2D) {
        .id = texture.id,
        .format = PixelFormatToRlPixelFormat(texture.format),
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
