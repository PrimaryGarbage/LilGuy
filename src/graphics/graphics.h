#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "short_types.h"
#include "result.h"
#include "color.h"
#include "vector2.h"
#include "rect.h"

Result graphics_init(const char* windowTitle, u32 windowWidth, u32 windowHeight);

// Returns false when window should close
bool graphics_update();

void graphics_deinit();

void graphics_clearWindow(Color color);
void graphics_drawRect(Rect rect, Color color, bool wireframe);
void graphics_drawSquare(Vector2 position, float size, Color color, bool wireframe);
void graphics_drawCircle(Vector2 position, float radius, Color color, bool wireframe);
void graphics_drawLine(Vector2 a, Vector2 b, float width, Color color);
Vector2 graphics_getMousePosition();

#endif // __GRAPHICS_H__