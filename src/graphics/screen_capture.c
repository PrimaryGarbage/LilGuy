#include "screen_capture.h"
#include <stdlib.h>
#include "image.h"
#include "result.h"

#ifdef _WIN32
  #include <windows.h>
  #include <wingdi.h>
#elif __linux__
  #include <string.h>
  #define STB_IMAGE_IMPLEMENTATION
  #include "stb_image.h"
  #include <stdio.h>
#else
  #error "Unsupported platform"
#endif


#ifdef _WIN32

Result CaptureScreen(Image* image_out)
{
    u32 width  = GetSystemMetrics(SM_CXSCREEN);
    u32 height = GetSystemMetrics(SM_CYSCREEN);
    
    int bufferSize = height * width * 4;  // 4 bytes per pixel (BGRA)
    u32* data = malloc(bufferSize);
    if (!data) return Error(RESULT_SCREEN_CAPTURE_ERROR, "Failed to create screen capture bitmap");
    
    HDC hdcScreen = GetDC(NULL);
    HDC hdcMem = CreateCompatibleDC(hdcScreen);
    
    // Create a DIB section with guaranteed 32-bit BGRA
    BITMAPINFO bmi = {0};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height;  // top-down
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;
    
    u8* bits = NULL;
    HBITMAP hBitmap = CreateDIBSection(hdcScreen, &bmi, DIB_RGB_COLORS, (void**)&bits, NULL, 0);
    if (!hBitmap || !bits) {
        // Fallback: use compatible bitmap + GetDIBits (rarely needed)
        free(data);
        ReleaseDC(NULL, hdcScreen);
        DeleteDC(hdcMem);
        return Error(RESULT_SCREEN_CAPTURE_ERROR, "Failed to create screen capture bitmap");
    }
    
    // Select the DIB into memory DC and copy screen
    HGDIOBJ oldBmp = SelectObject(hdcMem, hBitmap);
    BitBlt(hdcMem, 0, 0, width, height, hdcScreen, 0, 0, SRCCOPY);
    
    // Copy the BGRA data to our result buffer
    memcpy(data, bits, bufferSize);
    
    // Cleanup
    SelectObject(hdcMem, oldBmp);
    DeleteObject(hBitmap);
    DeleteDC(hdcMem);
    ReleaseDC(NULL, hdcScreen);

    *image_out = (Image){
        .data = data,
        .dataSize = bufferSize,
        .width = width,
        .height = height,
        .format = PIXEL_FORMAT_UNCOMPRESSED_R8G8B8A8,
    };

    Image_SwapRAndBChannels(image_out);

    return Success();
}

#endif

#ifdef __linux__

bool IsSpectacleAvailable() {
    FILE *fp = popen("spectacle --version 2>/dev/null", "r");
    if (!fp) return false;
    
    char buffer[128];
    int found = (fgets(buffer, sizeof(buffer), fp) != NULL);
    int status = pclose(fp);
    
    return found && status == 0;
}

Result CaptureScreen(Image* image_out)
{
    // 1. Check if spectacle available
    if (!IsSpectacleAvailable())
        return Error(RESULT_SCREEN_CAPTURE_ERROR, "Spectacle isn't available. This application requires spectacle to work");

    // 2. Use spectacle in background mode to capture the full screen and output the PNG data to stdout.
    FILE* pipe = popen("spectacle --background --current --nonotify --output /dev/stdout 2>/dev/null", "r");
    if (!pipe) {
        return Error(RESULT_SCREEN_CAPTURE_ERROR, "Failed to run spectacle");
    }

    // 3. Read the PNG data from the pipe into a memory buffer.
    u8* pngData = NULL;
    size_t pngSize = 0;
    size_t capacity = 4096;
    pngData = malloc(capacity);
    if (!pngData) {
        pclose(pipe);
        return Error(RESULT_SCREEN_CAPTURE_ERROR, "Failed to read the PNG data");
    }

    size_t bytesRead;
    while ((bytesRead = fread(pngData + pngSize, 1, capacity - pngSize, pipe)) > 0) {
        pngSize += bytesRead;
        if (pngSize == capacity) {
            capacity *= 2;
            u8* reallocatedData = realloc(pngData, capacity);
            if (!reallocatedData) {
                free(pngData);
                pclose(pipe);
                return Error(RESULT_MEMORY_ALLOCATION_ERROR, "Failed to realloc memory while reading png data");
            }
            pngData = reallocatedData;
        }
    }
    pclose(pipe);

    // 4. Decode the PNG data into RGBA format using stb_image.
    int width, height, channels;
    u32* data = (u32*)stbi_load_from_memory(pngData, (int)pngSize, &width, &height, &channels, 4);
    if (width <= 0 || height <= 0)
        return Error(RESULT_SCREEN_CAPTURE_ERROR, "Failed to load image from memory: invalid image dimensions");

    *image_out = (Image){
        .data = data,
        .dataSize = bytesRead,
        .width = width,
        .height = height,
        .format = PIXEL_FORMAT_UNCOMPRESSED_R8G8B8A8,
    };

    return Success();
}

#endif