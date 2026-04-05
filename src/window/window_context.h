#ifndef __WINDOW_CONTEXT_H__
#define __WINDOW_CONTEXT_H__

#include "short_types.h"
#include "vector2.h"

typedef struct mfb_window mfb_window;

typedef struct WindowContext {
    Vector2u windowSize;
    u32* buffer;
    size_t bufferSize;
    bool shouldClose;
} WindowContext;

#endif // __WINDOW_CONTEXT_H__