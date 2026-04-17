#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "graphics_transform.h"
#include "rect.h"
#include "physics/transform.h"
#include "vector2.h"
#include "color.h"
#include "image.h"
#include "texture2d.h"
#include "result.h"

Result Graphics_CaptureScreen(Image* image_out);

Vector2 Graphics_WorldToScreen(Vector2 vec);
Vector2 Graphics_ScreenToWorld(Vector2 vec);
Rect Graphics_WorldToScreenRect(Rect rect);
Rect Graphics_ScreenToWorldRect(Rect rect);

float Graphics_GetScreenHeight();
float Graphics_GetScreenWidth();
Vector2 Graphics_GetScreenSize();
Rect Graphics_GetScreenRect();
Vector2 Graphics_GetMousePosition();
Vector2 Graphics_GetMousePositionW();

void Graphics_SetTransform(GraphicsTransform transform);
void Graphics_SetTransformW(const Transform* transform);
void Graphics_ClearTransform();

void Graphics_DrawRect(Rect rect, Color color);
void Graphics_DrawRectT(Vector2 size, Color color);
void Graphics_DrawRectW(Rect rect, Color color);
void Graphics_DrawSquare(Vector2 position, float size, Color color);
void Graphics_DrawSquareW(Vector2 position, float size, Color color);
void Graphics_DrawCircle(Vector2 position, float radius, Color color);
void Graphics_DrawCircleT(float radius, Color color);
void Graphics_DrawCircleW(Vector2 position, float radius, Color color);
void Graphics_DrawLineSimple(Vector2 a, Vector2 b, Color color);
void Graphics_DrawLineSimpleW(Vector2 a, Vector2 b, Color color);
void Graphics_DrawLine(Vector2 a, Vector2 b, float width, Color color);
void Graphics_DrawLineW(Vector2 a, Vector2 b, float width, Color color);
void Graphics_DrawVector(Vector2 a, Vector2 b, Color color);
void Graphics_DrawVectorFromPoint(Vector2 point, Vector2 vec, Color color);
void Graphics_DrawVectorW(Vector2 a, Vector2 b, Color color);
void Graphics_DrawVectorFromPointW(Vector2 point, Vector2 vec, Color color);
Texture2D Graphics_LoadTextureFromImage(const Image* image);
Texture2D Graphics_LoadTexture(const char* imagePath);
void Graphics_UnloadTexture(Texture2D texture);
void Graphics_DrawTexture(const Texture2D* texture, Rect dest);
void Graphics_DrawTextureT(const Texture2D* texture);
void Graphics_DrawTintedTextureT(const Texture2D* texture, Color tint);
void Graphics_DrawTextureFullscreen(const Texture2D* texture);
void Graphics_ClearBackground(Color color);
void Graphics_Flush();

#endif // __GRAPHICS_H__