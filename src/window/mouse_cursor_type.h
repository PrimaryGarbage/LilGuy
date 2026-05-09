#ifndef __MOUSE_CURSOR_TYPE_H__
#define __MOUSE_CURSOR_TYPE_H__

typedef enum {
    MOUSE_CURSOR_TYPE_DEFAULT       = 0,     // Default pointer shape
    MOUSE_CURSOR_TYPE_ARROW         = 1,     // Arrow shape
    MOUSE_CURSOR_TYPE_IBEAM         = 2,     // Text writing cursor shape
    MOUSE_CURSOR_TYPE_CROSSHAIR     = 3,     // Cross shape
    MOUSE_CURSOR_TYPE_POINTING_HAND = 4,     // Pointing hand cursor
    MOUSE_CURSOR_TYPE_RESIZE_EW     = 5,     // Horizontal resize/move arrow shape
    MOUSE_CURSOR_TYPE_RESIZE_NS     = 6,     // Vertical resize/move arrow shape
    MOUSE_CURSOR_TYPE_RESIZE_NWSE   = 7,     // Top-left to bottom-right diagonal resize/move arrow shape
    MOUSE_CURSOR_TYPE_RESIZE_NESW   = 8,     // The top-right to bottom-left diagonal resize/move arrow shape
    MOUSE_CURSOR_TYPE_RESIZE_ALL    = 9,     // The omnidirectional resize/move cursor shape
    MOUSE_CURSOR_TYPE_NOT_ALLOWED   = 10     // The operation-not-allowed shape
} MouseCursorType;

#endif // __MOUSE_CURSOR_TYPE_H__