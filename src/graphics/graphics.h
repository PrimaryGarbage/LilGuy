#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "rect.h"
#include "scene/transform.h"
#include "vector2.h"
#include "color.h"
#include "image.h"
#include "texture2d.h"
#include "result.h"

Result Graphics_CaptureScreen(Image* image_out);

void Graphics_SetTransform(const Transform* transform);
void Graphics_ClearTransform();

void Graphics_DrawRect(Rect rect, Color color);
void Graphics_DrawSquare(Vector2 position, float size, Color color);
void Graphics_DrawCircle(Vector2 position, float radius, Color color);
void Graphics_DrawLine(Vector2 a, Vector2 b, float width, Color color);
Texture2D Graphics_LoadTextureFromImage(const Image* image);
void Graphics_UnloadTexture(Texture2D texture);
void Graphics_DrawTexture(const Texture2D* texture, Rect dest);
void Graphics_DrawTextureFullscreen(const Texture2D* texture);
void Graphics_ClearBackground(Color color);
void Graphics_Flush();

#endif // __GRAPHICS_H__