#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "vector2.h"

void Window_Init(const char* windowTitle, Vector2u windowSize);
void Window_PollEvents();
void Window_Destroy();
bool Window_ShouldClose();
void Window_Hide();
void Window_Show();

#endif // __WINDOW_H__