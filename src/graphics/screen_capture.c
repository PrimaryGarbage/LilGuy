#include "screen_capture.h"
#include <windows.h>
#include <wingdi.h>
#include "memory_s.h"

u32* CaptureScreen(u32* width_out, u32* height_out)
{
    *width_out  = GetSystemMetrics(SM_CXSCREEN);
    *height_out = GetSystemMetrics(SM_CYSCREEN);
    
    int bufferSize = (*width_out) * (*height_out) * 4;  // 4 bytes per pixel (BGRA)
    unsigned char* result = (unsigned char*)malloc(bufferSize);
    if (!result) return NULL;
    
    HDC hdcScreen = GetDC(NULL);
    HDC hdcMem = CreateCompatibleDC(hdcScreen);
    
    // Create a DIB section with guaranteed 32-bit BGRA
    BITMAPINFO bmi = {0};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = *width_out;
    bmi.bmiHeader.biHeight = -(*height_out);  // top-down
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    
    unsigned char* bits = NULL;
    HBITMAP hBitmap = CreateDIBSection(hdcScreen, &bmi, DIB_RGB_COLORS, (void**)&bits, NULL, 0);
    if (!hBitmap || !bits) {
        // Fallback: use compatible bitmap + GetDIBits (rarely needed)
        free(result);
        ReleaseDC(NULL, hdcScreen);
        DeleteDC(hdcMem);
        return NULL;
    }
    
    // Select the DIB into memory DC and copy screen
    HGDIOBJ oldBmp = SelectObject(hdcMem, hBitmap);
    BitBlt(hdcMem, 0, 0, *width_out, *height_out, hdcScreen, 0, 0, SRCCOPY);
    
    // Copy the BGRA data to our result buffer
    memcpy(result, bits, bufferSize);
    
    // Cleanup
    SelectObject(hdcMem, oldBmp);
    DeleteObject(hBitmap);
    DeleteDC(hdcMem);
    ReleaseDC(NULL, hdcScreen);
    
    return result;
}