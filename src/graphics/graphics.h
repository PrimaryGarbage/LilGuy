#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "rect.h"
#include "vector2.h"
#include "color.h"
#include "image.h"
#include "texture2d.h"

typedef struct WindowContext* WindowHandle; // FD

void Graphics_SetWindow(WindowHandle handle);

void Graphics_DrawRect(Rect rect, Color color, bool wireframe);
void Graphics_DrawSquare(Vector2 position, float size, Color color, bool wireframe);
void Graphics_DrawCircle(Vector2 position, float radius, Color color, bool wireframe);
void Graphics_DrawLine(Vector2 a, Vector2 b, float width, Color color);
void Graphics_DrawImage(const Image* image, Rect rect);
Texture2D Graphics_LoadTextureFromImage(const Image* image);
void Graphics_UnloadTexture(Texture2D texture);
void Graphics_DrawTexture(const Texture2D* texture, Rect dest);
void Graphics_DrawTextureFullscreen(const Texture2D* texture);

void Graphics_ClearWindow(Color color);

void Graphics_WriteImageToPngFile(const Image* image, const char* filepath);

#endif // __GRAPHICS_H__