#ifndef __GRAPHICS_CONTEXT_H__
#define __GRAPHICS_CONTEXT_H__

#include "MiniFB.h"
#include "short_types.h"
#include "vector2.h"
#include "image.h"

typedef struct mfb_window mfb_window;

typedef struct GraphicsContext {
    mfb_window* window;
    mfb_update_state windowUpdateState;
    Vector2_u windowSize;
    u32* buffer;
    size_t bufferSize;
    Vector2 mousePosition;
    Image screenCapture;
} GraphicsContext;

#endif // __GRAPHICS_CONTEXT_H__