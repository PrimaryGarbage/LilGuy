#ifndef __RAYLIB_WRAPPER_H__
#define __RAYLIB_WRAPPER_H__

#include "physics/matrix.h"
#include "short_types.h"
#include "vector2.h"
#include "graphics/color.h"
#include "graphics/image.h"
#include "graphics/texture2d.h"
#include "rect.h"

void Raylib_InitWindow(u32 windowWidth, u32 windowHeight, const char* windowTitle, bool fullscreen);
void Raylib_SetWindowMonitor(u8 monitor);
bool Raylib_WindowShouldClose();
void Raylib_CloseWindow();
void Raylib_PollInputEvents();
void Raylib_BeginDrawing();
void Raylib_EndDrawing();
void Raylib_SwapBuffers();
u32 Raylib_GetScreenWidth();
u32 Raylib_GetScreenHeight();
void Raylib_ClearBackground(Color color);
bool Raylib_IsKeyPressed(int key);
bool Raylib_IsKeyReleased(int key);
bool Raylib_IsKeyDown(int key);
bool Raylib_IsMouseButtonPressed(int button);
bool Raylib_IsMouseButtonReleased(int button);
bool Raylib_IsMouseButtonDown(int button);
Vector2 Raylib_GetMousePosition();
Image Raylib_LoadImageFromScreen();
Image Raylib_LoadImage(const char* path);
Image Raylib_ImageFromImage(const Image* image, Rect rect);
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

// Collision
bool Raylib_CheckCollisionRects(Rect a, Rect b);
bool Raylib_CheckCollisionPointRect(Vector2 point, Rect rect);
Rect Raylib_GetCollisionRect(Rect a, Rect b);
bool CheckCollisionPointRect(Vector2 point, Rect rect); 

#endif // __RAYLIB_WRAPPER_H__