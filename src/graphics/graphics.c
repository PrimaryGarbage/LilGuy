#include "graphics.h"
#include "graphics/color.h"
#include "graphics/image.h"
#include "result.h"
#include "stb_image_write.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "raylib_wrapper.h"

void Graphics_DrawRect(Rect rect, Vector2 origin, Color color)
{
    Raylib_DrawRectanglePro(rect, Vector2_Zero(), 0.0f, color);
}

void Graphics_DrawSquare(Vector2 position, Vector2 origin, float size, Color color)
{
    Graphics_DrawRect((Rect){ .a = position, .b = Vector2_AddScalar(position, size)}, origin, color);
}

void Graphics_DrawCircle(Vector2 position, float radius, Color color)
{
    Raylib_DrawCircleV(position, radius, color);
}

void Graphics_DrawLine(Vector2 a, Vector2 b, float width, Color color)
{
    Raylib_DrawLineEx(a, b, width, color);
}

Texture2D Graphics_LoadTextureFromImage(const Image* image)
{
    return Raylib_LoadTextureFromImage(image);
}

void Graphics_UnloadTexture(Texture2D texture)
{
    Raylib_UnloadTexture(texture);
}

void Graphics_DrawTexture(const Texture2D* texture, Rect dest)
{
    Raylib_DrawTexturePro(*texture, (Rect){ 0.0f, 0.0f, texture->width, texture->height }, dest, (Vector2){ 0.0f, 0.0f }, 0.0f, COLOR_WHITE);
}

void Graphics_DrawTextureFullscreen(const Texture2D* texture)
{
    Rect dest = { Vector2_Zero(), { Raylib_GetScreenWidth(), Raylib_GetScreenHeight() } };
    Raylib_DrawTexturePro(*texture, (Rect){ 0.0f, 0.0f, texture->width, texture->height }, dest, (Vector2){ 0.0f, 0.0f }, 0.0f, COLOR_WHITE);
}

void Window_WriteImageToPngFile(const Image* image, const char* filepath)
{
    switch(image->format)
    {
        case PIXEL_FORMAT_RGBA:
        {
            stbi_write_png(filepath, image->width, image->height, 4, image->data, image->width * 4);
            break;
        }
        case PIXEL_FORMAT_BGRA:
        {
            size_t pixelCount = image->width * image->height;
            u32* tempBuffer = malloc(pixelCount * 4);
            if(!tempBuffer) MEMORY_PANIC();

            for (size_t i = 0; i < pixelCount; ++i)
                tempBuffer[i] = Color_SwapRAndB(image->data[i]);

            stbi_write_png(filepath, image->width, image->height, 4, tempBuffer, image->width * 4);
            free(tempBuffer);
            break;
        }
    }
}

void Graphics_ClearBackground(Color color)
{
    Raylib_ClearBackground(color);
}

void Graphics_Flush()
{
    Raylib_rlDrawRenderBatchActive();
    Raylib_SwapBuffers();
}