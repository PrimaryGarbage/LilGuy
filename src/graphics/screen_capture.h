#ifndef __SCREEN_CAPTURE_H__
#define __SCREEN_CAPTURE_H__

#include "short_types.h"

// User must call free() on the returned u32 pixel array
u32* CaptureScreen(u32* width_out, u32* height_out);

#endif // __SCREEN_CAPTURE_H__