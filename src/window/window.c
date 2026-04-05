#include "window.h"
#include <math.h>
#include "raylib_wrapper.h"

void Window_Init(const char* windowTitle, Vector2u windowSize)
{
    Raylib_InitWindow(windowSize.x, windowSize.y, windowTitle);
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
        .a = { 0.0f, 0.0f, },
        .b = { Raylib_GetScreenWidth(), Raylib_GetScreenHeight() }
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