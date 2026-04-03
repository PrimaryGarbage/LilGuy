#ifndef __SCREEN_CAPTURE_H__
#define __SCREEN_CAPTURE_H__

#include "short_types.h"
#include "result.h"

// User must call free() on the returned u32 data buffer
Result CaptureScreen(u32* width_out, u32* height_out, u32** data_out);

#endif // __SCREEN_CAPTURE_H__