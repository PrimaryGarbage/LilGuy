#include "screen_capture.h"
#include "image.h"
#include <stdlib.h>

#ifdef _WIN32
  #include <windows.h>
  #include <wingdi.h>
#elif __linux__
  #include <string.h>
  #include "stb_image.h"
  #include <stdio.h>
  #include "graphics/color.h"
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

Result CaptureScreen(u32* width_out, u32* height_out, u32** data_out)
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
    unsigned char* png_data = NULL;
    size_t png_size = 0;
    size_t capacity = 4096;
    png_data = (unsigned char*)malloc(capacity);
    if (!png_data) {
        pclose(pipe);
        return Error(RESULT_SCREEN_CAPTURE_ERROR, "Failed to read the PNG data");
    }

    size_t bytes_read;
    while ((bytes_read = fread(png_data + png_size, 1, capacity - png_size, pipe)) > 0) {
        png_size += bytes_read;
        if (png_size == capacity) {
            capacity *= 2;
            u8* new_data = realloc(png_data, capacity);
            if (!new_data) {
                free(png_data);
                pclose(pipe);
                return Error(RESULT_MEMORY_ALLOCATION_ERROR, "Failed to realloc memory while reading png data");
            }
            png_data = new_data;
        }
    }
    pclose(pipe);

    // 4. Decode the PNG data into RGBA format using stb_image.
    int width, height, channels;
    *data_out = (u32*)stbi_load_from_memory(png_data, (int)png_size, &width, &height, &channels, 4);

    *width_out = width;
    *height_out = height;

    for(size_t i = 0; i < *width_out * *height_out; ++i)
        (*data_out)[i] = Color_SwapRAndB(result[i]);

    return Success();
}

#endif