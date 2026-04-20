#include "graphics.h"
#include "draw_order.h"
#include "graphics/color.h"
#include "graphics/image.h"
#include "physics/matrix.h"
#include "rect.h"
#include "result.h"
#include "stb_image_write.h"
#include <assert.h>
#include <basetsd.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include "raylib_wrapper.h"
#include "screen_capture.h"
#include "vector2.h"
#include "deferred_rendering.h"

static Matrix s_modelMatrix;

static RenderCall s_renderCalls[512];
static u32 s_renderCallsCount = 0u;

static void QueueRenderCall(i32 drawOrder, RenderCallFunction function, const union RenderCallFunctionArguments* functionArguments)
{
    s_renderCalls[s_renderCallsCount] = (RenderCall){
        .drawOrder = drawOrder,
        .drawIndex = s_renderCallsCount,
        .function = function,
        .functionArguments = *functionArguments
    };

    s_renderCallsCount++;
}

Result Graphics_CaptureScreen(Image* image_out)
{
    return CaptureScreen(image_out);
}

float Graphics_GetScreenHeight()
{
    return Raylib_GetScreenHeight();
}

float Graphics_GetScreenWidth()
{
    return Raylib_GetScreenWidth();
}

Vector2 Graphics_GetScreenSize()
{
    return (Vector2){ Raylib_GetScreenWidth(), Raylib_GetScreenHeight() };
}

Rect Graphics_GetScreenRect()
{
    return (Rect) {
        .x = 0.0f, 
        .y = 0.0f,
        .width = Raylib_GetScreenWidth(), 
        .height = Raylib_GetScreenHeight() 
    };
}

void Graphics_SetModelMatrix(const Transform* transform)
{
    Matrix originTranslation = Matrix_Translate((Vector2){ .x = -transform->origin.x, .y = -transform->origin.y });
    s_modelMatrix = Matrix_Mult(&originTranslation, &transform->matrix);
}

void Graphics_ClearModelMatrix()
{
    s_modelMatrix = Matrix_Identity();
}

void _DrawRect_RenderCall(union RenderCallFunctionArguments arguments)
{
    DrawRectArguments args = arguments.drawRectArguments;
    Raylib_DrawRectanglePro(args.rect, Vector2_Zero(), 0.0f, args.color);
}

void Graphics_DrawRect(Rect rect, Color color, i32 drawOrder)
{
    DrawRectArguments args = {
        .rect = rect,
        .color = color
    };

    QueueRenderCall(drawOrder, _DrawRect_RenderCall, (union RenderCallFunctionArguments*)&args);
}

static void _DrawRectT_RenderCall(union RenderCallFunctionArguments arguments)
{
    DrawRectTArguments args = arguments.drawRectTArguments;

    Raylib_PushMatrix(&args.matrix);

    Raylib_DrawRectanglePro(Rect_FromVectors(Vector2_Zero(), args.size), Vector2_Zero(), 0.0f, args.color);

    Raylib_PopMatrix();
}

void Graphics_DrawRectT(Vector2 size, Color color, i32 drawOrder)
{
    DrawRectTArguments args = {
        .matrix = s_modelMatrix,
        .size = size,
        .color = color
    };

    QueueRenderCall(drawOrder, _DrawRectT_RenderCall, (union RenderCallFunctionArguments*)&args);
} 

void Graphics_DrawSquare(Vector2 position, float size, Color color, i32 drawOrder)
{
    Graphics_DrawRect((Rect){
         .x = position.x,
         .y = position.y,
         .width = size,
         .height = size
    },
    color, drawOrder);
}

void _DrawCircle_RenderCall(union RenderCallFunctionArguments arguments)
{
    DrawCircleArguments args = arguments.drawCircleArguments;
    Raylib_DrawCircleV(args.position, args.radius, args.color);
}

void Graphics_DrawCircle(Vector2 position, float radius, Color color, i32 drawOrder)
{
    DrawCircleArguments args = {
        .position = position,
        .radius = radius,
        .color = color
    };

    QueueRenderCall(drawOrder, _DrawCircle_RenderCall, (union RenderCallFunctionArguments*)&args);
}

void _DrawCircleT_RenderCall(union RenderCallFunctionArguments arguments)
{
    DrawCircleTArguments args = arguments.drawCircleTArguments;

    Raylib_PushMatrix(&args.matrix);
    Raylib_DrawCircleV(Vector2_Zero(), args.radius, args.color);
    Raylib_PopMatrix();
}

void Graphics_DrawCircleT(float radius, Color color, i32 drawOrder)
{
    DrawCircleTArguments args = {
        .matrix = s_modelMatrix,
        .radius = radius,
        .color = color
    };

    QueueRenderCall(drawOrder, _DrawCircleT_RenderCall, (union RenderCallFunctionArguments*)&args);
}

void _DrawLineSimple_RenderCall(union RenderCallFunctionArguments arguments)
{
    DrawLineSimpleArguments args = arguments.drawLineSimpleArguments;
    Raylib_DrawLineV(args.a, args.b, args.color);
}

void Graphics_DrawLineSimple(Vector2 a, Vector2 b, Color color)
{
    DrawLineSimpleArguments args = {
        .a = a,
        .b = b,
        .color = color
    };

    QueueRenderCall(DRAW_ORDER_TOP, _DrawLineSimple_RenderCall, (union RenderCallFunctionArguments*)&args);
}

void _DrawLine_RenderCall(union RenderCallFunctionArguments arguments)
{
    DrawLineArguments args = arguments.drawLineArguments;
    Raylib_DrawLineEx(args.a, args.b, args.width, args.color);
}

void Graphics_DrawLine(Vector2 a, Vector2 b, float width, Color color)
{
    DrawLineArguments args = {
        .a = a,
        .b = b,
        .width = width,
        .color = color
    };

    QueueRenderCall(DRAW_ORDER_TOP, _DrawLine_RenderCall, (union RenderCallFunctionArguments*)&args);
}

void _DrawVector_RenderCall(union RenderCallFunctionArguments arguments)
{
    DrawVectorArguments args = arguments.drawVectorArguments;

    constexpr float arrowSize = 15.0f;
    constexpr float arrowAngle = 20.0f;
    Vector2 arrowVec = Vector2_Normalize(Vector2_Sub(args.a, args.b));

    Raylib_DrawLineV(args.a, args.b, args.color);
    Raylib_DrawLineV(args.b, Vector2_Add(args.b, Vector2_MultScalar(Vector2_Rotate(arrowVec, arrowAngle), arrowSize)), args.color);
    Raylib_DrawLineV(args.b, Vector2_Add(args.b, Vector2_MultScalar(Vector2_Rotate(arrowVec, -arrowAngle), arrowSize)), args.color);
}

void Graphics_DrawVector(Vector2 a, Vector2 b, Color color)
{
    DrawVectorArguments args = {
        .a = a,
        .b = b,
        .color = color
    };

    QueueRenderCall(DRAW_ORDER_TOP, _DrawVector_RenderCall, (union RenderCallFunctionArguments*)&args);
}

void _DrawVectorFromPoint_RenderCall(union RenderCallFunctionArguments arguments)
{
    DrawVectorFromPointArguments args = arguments.drawVectorFromPointArguments;

    constexpr float arrowSize = 15.0f;
    constexpr float arrowAngle = 20.0f;
    Vector2 sum = Vector2_Add(args.point, args.vec);
    Vector2 arrowVec = Vector2_Normalize(Vector2_MultScalar(args.vec, -1.0f));

    Raylib_DrawLineV(args.point, sum, args.color);
    Raylib_DrawLineV(sum, Vector2_Add(sum, Vector2_MultScalar(Vector2_Rotate(arrowVec, arrowAngle), arrowSize)), args.color);
    Raylib_DrawLineV(sum, Vector2_Add(sum, Vector2_MultScalar(Vector2_Rotate(arrowVec, -arrowAngle), arrowSize)), args.color);
}

void Graphics_DrawVectorFromPoint(Vector2 point, Vector2 vec, Color color)
{
    DrawVectorFromPointArguments args = {
        .point = point,
        .vec = vec,
        .color = color
    };

    QueueRenderCall(DRAW_ORDER_TOP, _DrawVectorFromPoint_RenderCall, (union RenderCallFunctionArguments*)&args);
}

Texture2D Graphics_LoadTextureFromImage(const Image* image)
{
    return Raylib_LoadTextureFromImage(image);
}

Texture2D Graphics_LoadTexture(const char* imagePath)
{
    return Raylib_LoadTexture(imagePath);
}

void Graphics_UnloadTexture(Texture2D texture)
{
    Raylib_UnloadTexture(texture);
}

void _DrawTexture_RenderCall(union RenderCallFunctionArguments arguments)
{
    DrawTextureArguments args = arguments.drawTextureArguments;

    Raylib_DrawTexturePro(*args.texture, (Rect){ 0.0f, 0.0f, args.texture->width, args.texture->height }, args.dest, (Vector2){ 0.0f, 0.0f }, 0.0f, COLOR_WHITE);
}

void Graphics_DrawTexture(const Texture2D* texture, Rect dest, i32 drawOrder)
{
    DrawTextureArguments args = {
        .texture = texture,
        .dest = dest,
    };

    QueueRenderCall(drawOrder, _DrawTexture_RenderCall, (union RenderCallFunctionArguments*)&args);
}

void _DrawTextureT_RenderCall(union RenderCallFunctionArguments arguments)
{
    DrawTextureTArguments args = arguments.drawTextureTArguments;

    Raylib_PushMatrix(&args.matrix);

    Raylib_DrawTexture(*args.texture, 0.0f, 0.0f, args.tint);

    //Rect src = (Rect){ .x = 0.0f, .y = 0.0f, .width = args.texture->width, .height = args.texture->height };
    //Rect dest = (Rect){ .x = args.transform.position.x, .y = args.transform.position.y, .width = args.texture->width * args.transform.scale.x, .height = args.texture->height * args.transform.scale.y };
    //Vector2 origin = Vector2_Mult(WorldToScreenOrigin(args.transform.origin, (Vector2){ .x = args.texture->width, .y = args.texture->height }), args.transform.scale);

    //Raylib_DrawTexturePro(*args.texture, src, dest, origin, -args.transform.rotation, COLOR_WHITE);

    Raylib_PopMatrix();
}

void Graphics_DrawTextureT(const Texture2D* texture, i32 drawOrder, Color tint)
{
    DrawTextureTArguments args = {
        .matrix = s_modelMatrix,
        .tint = tint,
        .texture = texture,
    };

    QueueRenderCall(drawOrder, _DrawTextureT_RenderCall, (union RenderCallFunctionArguments*)&args);
}

void Graphics_ClearBackground(Color color)
{
    Raylib_ClearBackground(color);
}

void Graphics_Flush()
{
    qsort(s_renderCalls, s_renderCallsCount, sizeof(RenderCall), RenderCallComparer);

    for(u32 i = 0; i < s_renderCallsCount; ++i)
        s_renderCalls[i].function(s_renderCalls[i].functionArguments);

    s_renderCallsCount = 0u;

    Raylib_rlDrawRenderBatchActive();
    Raylib_SwapBuffers();
}

Vector2 Graphics_GetMousePosition()
{
    return Raylib_GetMousePosition();
}