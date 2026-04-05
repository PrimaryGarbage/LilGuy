#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "result.h"
#include "short_types.h"
#include "rect.h"

typedef struct WindowContext* WindowHandle; // FD
typedef struct Image Image;                 // FD

Result Window_Init(const char* windowTitle, u32 windowWidth, u32 windowHeight, WindowHandle* handle_out);
// Returns false when window should close
bool Window_Update(WindowHandle handle);
// Also polls events
void Window_DrawBuffer(WindowHandle handle);
void Window_PollEvents(WindowHandle handle);
void Window_Destroy(WindowHandle handle);

Rect Window_GetWindowRect(WindowHandle handle);
bool Window_ShouldClose(WindowHandle handle);

#endif // __WINDOW_H__