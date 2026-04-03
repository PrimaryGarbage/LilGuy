#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "short_types.h"
#include "result.h"
#include "color.h"
#include "vector2.h"
#include "rect.h"

typedef struct Image Image; // FD

Result Graphics_Init(const char* windowTitle, u32 windowWidth, u32 windowHeight);
// Returns false when window should close
bool Graphics_Update();
void Graphics_Deinit();

void Graphics_ClearWindow(Color color);
void Graphics_ClearWindowWithImage(const Image* image);
void Graphics_DrawRect(Rect rect, Color color, bool wireframe);
void Graphics_DrawSquare(Vector2 position, float size, Color color, bool wireframe);
void Graphics_DrawCircle(Vector2 position, float radius, Color color, bool wireframe);
void Graphics_DrawLine(Vector2 a, Vector2 b, float width, Color color);
void Graphics_WaitSync();
Vector2 Graphics_GetMousePosition();
Image* Graphics_GetScreenCaptureImage();
void Graphics_DrawImage(const Image* image, Rect rect);

void Graphics_WriteImageToPngFile(const Image* image, const char* filepath);
Rect Graphics_GetWindowRect();

#endif // __GRAPHICS_H__