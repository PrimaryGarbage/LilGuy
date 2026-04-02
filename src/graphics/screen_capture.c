#include "screen_capture.h"
#include <windows.h>
#include <wingdi.h>
#include "memory_s.h"

u32* CaptureScreen(u32* width_out, u32* height_out)
{
    // Get screen dimensions
    *width_out  = GetSystemMetrics(SM_CXSCREEN);
    *height_out = GetSystemMetrics(SM_CYSCREEN);
    
    // Get screen DC and create compatible DC + bitmap
    HDC hdcScreen = GetDC(NULL);
    HDC hdcMem    = CreateCompatibleDC(hdcScreen);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, *width_out, *height_out);
    SelectObject(hdcMem, hBitmap);
    
    // Copy screen pixels to bitmap
    BitBlt(hdcMem, 0, 0, *width_out, *height_out, hdcScreen, 0, 0, SRCCOPY);
    
    // Prepare BITMAPINFO for 32-bit BGRA
    BITMAPINFO bmi = {0};
    bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth       = *width_out;
    bmi.bmiHeader.biHeight      = -(*height_out);  // Negative for top-down (matches screen orientation)
    bmi.bmiHeader.biPlanes      = 1;
    bmi.bmiHeader.biBitCount    = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    
    // Allocate buffer for pixel data
    int bufferSize = (*width_out) * (*height_out) * 4;  // 4 bytes per pixel (BGRA)
    u32* pixels = malloc_s(bufferSize);
    
    // Get the BGRA data from the bitmap
    GetDIBits(hdcScreen, hBitmap, 0, *height_out, pixels, &bmi, DIB_RGB_COLORS);
    
    // Cleanup GDI objects
    DeleteObject(hBitmap);
    DeleteDC(hdcMem);
    ReleaseDC(NULL, hdcScreen);
    
    return pixels;
}