#ifndef __WINDOW_CONTEXT_H__
#define __WINDOW_CONTEXT_H__

#include "MiniFB_enums.h"
#include "short_types.h"
#include "vector2.h"
#include "graphics/image.h"

typedef struct mfb_window mfb_window;

typedef struct WindowContext {
    mfb_window* mfbWindow;
    mfb_update_state windowUpdateState;
    Vector2u windowSize;
    u32* buffer;
    size_t bufferSize;
    Vector2 mousePosition;
    Image screenCapture;
} WindowContext;

#endif // __WINDOW_CONTEXT_H__