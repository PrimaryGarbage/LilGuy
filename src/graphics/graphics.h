#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "physics/transform.h"
#include "rect.h"
#include "vector2.h"
#include "color.h"
#include "image.h"
#include "texture2d.h"
#include "result.h"

Result Graphics_CaptureScreen(Image* image_out);

float Graphics_GetScreenHeight();
float Graphics_GetScreenWidth();
Vector2 Graphics_GetScreenSize();
Rect Graphics_GetScreenRect();
Vector2 Graphics_GetMousePosition();

void Graphics_SetModelMatrix(const Transform* transform);
void Graphics_ClearModelMatrix();

void Graphics_DrawRect(Rect rect, Color color, i32 drawOrder);
void Graphics_DrawRectT(Vector2 size, Color color, i32 drawOrder);
void Graphics_DrawSquare(Vector2 position, float size, Color color, i32 drawOrder);
void Graphics_DrawCircle(Vector2 position, float radius, Color color, i32 drawOrder);
void Graphics_DrawCircleT(float radius, Color color, i32 drawOrder);
void Graphics_DrawLineSimple(Vector2 a, Vector2 b, Color color);
void Graphics_DrawLine(Vector2 a, Vector2 b, float width, Color color);
void Graphics_DrawVector(Vector2 a, Vector2 b, Color color);
void Graphics_DrawVectorFromPoint(Vector2 point, Vector2 vec, Color color);
Texture2D Graphics_LoadTextureFromImage(const Image* image);
Texture2D Graphics_LoadTexture(const char* imagePath);
void Graphics_UnloadTexture(Texture2D texture);
void Graphics_DrawTexture(const Texture2D* texture, Rect dest, i32 drawOrder);
void Graphics_DrawTextureT(const Texture2D* texture, i32 drawOrder, Color tint);
void Graphics_ClearBackground(Color color);
void Graphics_Flush();

#endif // __GRAPHICS_H__