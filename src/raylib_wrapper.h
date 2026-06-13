#ifndef __RAYLIB_WRAPPER_H__
#define __RAYLIB_WRAPPER_H__

#include "graphics/font.h"
#include "graphics/shader.h"
#include "input/input_button.h"
#include "physics/matrix.h"
#include "short_types.h"
#include "vector2.h"
#include "graphics/color.h"
#include "graphics/image.h"
#include "graphics/texture2d.h"
#include "rect.h"

void Raylib_InitWindow(u32 windowWidth, u32 windowHeight, const char* windowTitle, bool fullscreen);
void Raylib_SetExitKey(InputKey key);
void Raylib_SetWindowMonitor(u8 monitor);
bool Raylib_WindowShouldClose();
void Raylib_CloseWindow();
void Raylib_SetMouseCursor(i32 cursor);
void Raylib_PollInputEvents();
void Raylib_BeginDrawing();
void Raylib_EndDrawing();
void Raylib_SwapBuffers();
void Raylib_BeginBlendMode(i32 mode);
void Raylib_EndBlendMode();
u32 Raylib_GetScreenWidth();
u32 Raylib_GetScreenHeight();
void Raylib_ClearBackground(Color color);
bool Raylib_IsKeyPressed(int key);
bool Raylib_IsKeyReleased(int key);
bool Raylib_IsKeyDown(int key);
bool Raylib_IsMouseButtonPressed(int button);
bool Raylib_IsMouseButtonReleased(int button);
bool Raylib_IsMouseButtonDown(int button);
float Raylib_GetMouseWheelMove();
Vector2 Raylib_GetMousePosition();
Image Raylib_LoadImageFromScreen();
Image Raylib_LoadImage(const char* path);
Image Raylib_ImageFromImage(const Image* image, Rect rect);
Image Raylib_GenImageColor(u32 width, u32 height, Color color);
void Raylib_ImageDrawPixel(Image* image, u32 x, u32 y, Color color);
void Raylib_ImageFlipHorizontal(Image* image);
void Raylib_ImageFlipVertical(Image* image);
Texture2D Raylib_LoadTextureFromImage(const Image* image);
Texture2D Raylib_LoadTexture(const char* imagePath);
void Raylib_UnloadTexture(Texture2D texture);
void Raylib_rlDrawRenderBatchActive();
float Raylib_GetFrameTime();
void Raylib_HideWindow();
void Raylib_ShowWindow();

// Draw functions
void Raylib_DrawTexture(Texture2D texture, float posX, float posY, Color tint);
void Raylib_DrawTexturePro(Texture2D texture, Rect source, Rect dest, Vector2 origin, float rotation, Color tint);
void Raylib_DrawRectangleV(Vector2 position, Vector2 size, Color color);
void Raylib_DrawRectanglePro(Rect rect, Vector2 origin, float rotation, Color color);
void Raylib_DrawLineV(Vector2 startPos, Vector2 endPos, Color color);
void Raylib_DrawLineEx(Vector2 startPos, Vector2 endPos, float thick, Color color);
void Raylib_DrawCircleV(Vector2 position, float radius, Color color);
void Raylib_PushMatrix(const Matrix* matrix);
void Raylib_PopMatrix();
void Raylib_DrawText(const char* text, u32 posX, u32 posY, u32 fontSize, Color color);
void Raylib_DrawTextPro(Font font, const char* text, Vector2 position, Vector2 origin, float rotation, float fontSize, float spacing, Color tint);

// Shaders
Shader Raylib_LoadShader(const char* vsFilePath, const char* fsFilePath);
void Raylib_UnloadShader(Shader* shader);
void Raylib_BeginShaderMode(const Shader* shader);
void Raylib_EndShaderMode();
bool Raylib_IsShaderValid(const Shader* shader);
int Raylib_GetShaderLocation(const Shader* shader, const char* uniformName);
int Raylib_GetShaderLocationAttrib(const Shader* shader, const char* attribName);
void Raylib_SetShaderValue(const Shader* shader, int locIndex, const void* value, UniformDataType uniformType);
void Raylib_SetShaderValueV(const Shader* shader, int locIndex, const void* value, UniformDataType uniformType, int count);
void Raylib_SetShaderValueMatrix(const Shader* shader, int locIndex, const Matrix* mat);
void Raylib_SetShaderValueTexture(const Shader* shader, int locIndex, const Texture2D* texture);

// Collision
bool Raylib_CheckCollisionRects(Rect a, Rect b);
bool Raylib_CheckCollisionPointRect(Vector2 point, Rect rect);
Rect Raylib_GetCollisionRect(Rect a, Rect b);

Vector2 Raylib_MeasureText(const char* text, float fontSize);
Font Raylib_GetFontDefault();

#endif // __RAYLIB_WRAPPER_H__