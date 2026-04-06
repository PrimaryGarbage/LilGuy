#ifndef __SCREEN_CAPTURE_H__
#define __SCREEN_CAPTURE_H__

#include "result.h"
#include "image.h"

// User must call Image_Free() on the returned image
Result CaptureScreen(Image* image_out);

#endif // __SCREEN_CAPTURE_H__