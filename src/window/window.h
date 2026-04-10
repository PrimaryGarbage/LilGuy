#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "vector2.h"
#include "rect.h"

void Window_Init(const char* windowTitle, Vector2u windowSize, bool fullscreen);
void Window_SetMonitor(u8 monitor);
void Window_PollEvents();
void Window_Destroy();
bool Window_ShouldClose();
void Window_Hide();
void Window_Show();
Rect Window_GetWindowRect();

#endif // __WINDOW_H__