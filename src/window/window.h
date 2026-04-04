#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "result.h"
#include "short_types.h"
#include "rect.h"
#include "window_callbacks.h"

typedef struct WindowContext* WindowHandle; // FD
typedef struct Image Image;                 // FD


Result Window_Init(const char* windowTitle, u32 windowWidth, u32 windowHeight, WindowHandle* handle_out);
// Returns false when window should close
bool Window_Update(WindowHandle handle);
void Window_Destroy(WindowHandle handle);
void Window_WaitSync(WindowHandle handle);

// Callbacks are called right before updating window
void Window_AddOnUpdateCallback(WindowHandle handle, Window_OnUpdateCallback callback);

Rect Window_GetWindowRect(WindowHandle handle);

#endif // __WINDOW_H__