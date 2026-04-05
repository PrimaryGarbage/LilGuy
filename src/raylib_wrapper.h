#ifndef __RAYLIB_WRAPPER_H__
#define __RAYLIB_WRAPPER_H__

#include "short_types.h"
#include "vector2.h"
#include "graphics/color.h"
#include "graphics/image.h"
#include "graphics/texture2d.h"
#include "rect.h"

void Raylib_InitWindow(u32 windowWidth, u32 windowHeight, const char* windowTitle);
bool Raylib_WindowShouldClose();
void Raylib_CloseWindow();
void Raylib_PollInputEvents();
void Raylib_BeginDrawing();
void Raylib_EndDrawing();
void Raylib_SwapBuffers();
void Raylib_ClearBackgroud(Color color);
bool Raylib_IsKeyPressed(int key);
bool Raylib_IsKeyReleased(int key);
bool Raylib_IsKeyDown(int key);
bool Raylib_IsMouseButtonPressed(int button);
bool Raylib_IsMouseButtonReleased(int button);
bool Raylib_IsMouseButtonDown(int button);
Vector2 Raylib_GetMousePosition();
Image Raylib_LoadImageFromScreen();
Texture2D Raylib_LoadTextureFromImage(const Image* image);
void Raylib_UnloadTexture(Texture2D texture);
void Raylib_DrawTexturePro(Texture2D texture, Rect source, Rect dest, Vector2 origin, float rotation, Color tint);
void Raylib_rlDrawRenderBatchActive();

#endif // __RAYLIB_WRAPPER_H__