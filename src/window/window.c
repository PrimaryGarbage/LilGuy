#include "window.h"
#include <math.h>
#include "raylib_wrapper.h"

void Window_Init(const char* windowTitle, Vector2u windowSize, bool fullscreen)
{
    Raylib_InitWindow(windowSize.x, windowSize.y, windowTitle, fullscreen);
}

void Window_SetMonitor(u8 monitor)
{
    Raylib_SetWindowMonitor(monitor);
}

void Window_PollEvents()
{
    Raylib_PollInputEvents();
}

void Window_Destroy()
{
    Raylib_CloseWindow();
}

Rect Window_GetWindowRect()
{
    return (Rect) {
        .x = 0.0f, 
        .y = 0.0f,
        .width = Raylib_GetScreenWidth(), 
        .height = Raylib_GetScreenHeight() 
    };
}

bool Window_ShouldClose()
{
    return Raylib_WindowShouldClose();
}

void Window_Hide()
{
    Raylib_HideWindow();
}

void Window_Show()
{
    Raylib_ShowWindow();
}