#include "graphics.h"
#include "draw_order.h"
#include "graphics/color.h"
#include "graphics/image.h"
#include "graphics_transform.h"
#include "rect.h"
#include "result.h"
#include "physics/transform.h"
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

constexpr GraphicsTransform c_defaultTransform = {
    .position = (Vector2){ 0.0f, 0.0f },
    .rotation = 0.0f,
    .scale = (Vector2){ 1.0f, 1.0f },
    .origin = (Vector2){ 0.0f, 0.0f },
};

static GraphicsTransform s_transform = c_defaultTransform;

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

// Transform lower-left corner origin to upper-left corner.
// Has to be a sepate function because Transform doesn't contain any size info.
static Vector2 WorldToScreenOrigin(Vector2 worldOrigin, Vector2 rectSize)
{
    return (Vector2){ .x = worldOrigin.x, .y = rectSize.y - worldOrigin.y };
}

Result Graphics_CaptureScreen(Image* image_out)
{
    return CaptureScreen(image_out);
}

Vector2 Graphics_WorldToScreen(Vector2 vec)
{
    return (Vector2){ vec.x, Graphics_GetScreenHeight() - vec.y };
}

Vector2 Graphics_ScreenToWorld(Vector2 vec)
{
    return (Vector2){ vec.x, Graphics_GetScreenHeight() - vec.y };
}

Rect Graphics_WorldToScreenRect(Rect rect)
{
    return (Rect) {
        .x = rect.x,
        .y = Graphics_GetScreenHeight()- rect.y - rect.height,
        .width = rect.width,
        .height = rect.height
    };
}

Rect Graphics_ScreenToWorldRect(Rect rect)
{
    return (Rect) {
        .x = rect.x,
        .y = Graphics_GetScreenHeight() - rect.y - rect.height,
        .width = rect.width,
        .height = rect.height
    };
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

void Graphics_SetTransform(GraphicsTransform transform)
{
    s_transform = transform;
}

void Graphics_SetTransformW(const Transform* transform)
{
    Graphics_SetTransform((GraphicsTransform) {
        .position = Graphics_WorldToScreen(transform->position),
        .rotation = transform->rotation,
        .scale = transform->scale,
        .origin = (Vector2){ .x = transform->origin.x, .y = transform->origin.y},
    });
}

void Graphics_ClearTransform()
{
    s_transform = c_defaultTransform;
}

void _DrawRect_RenderCall(union RenderCallFunctionArguments arguments)
{
    DrawRectArguments args = arguments.drawRectArguments;
    Raylib_DrawRectanglePro(args.rect, Vector2_Zero(), 0.0f, args.color);
}

void Graphics_DrawRect(Rect rect, Color color)
{
    DrawRectArguments args = {
        .rect = rect,
        .color = color
    };

    QueueRenderCall(DRAW_ORDER_DEFAULT, _DrawRect_RenderCall, (union RenderCallFunctionArguments*)&args);
}

static void _DrawRectT_RenderCall(union RenderCallFunctionArguments arguments)
{
    DrawRectTArguments args = arguments.drawRectTArguments;

    Vector2 origin = WorldToScreenOrigin(args.transform.origin, args.size);

    Raylib_DrawRectanglePro((Rect){ args.transform.position.x, args.transform.position.y, args.size.x * args.transform.scale.x, args.size.y * args.transform.scale.y}, 
        Vector2_Mult(origin, args.transform.scale), -args.transform.rotation, args.color);
}

void Graphics_DrawRectT(Vector2 size, Color color, i32 drawOrder)
{
    DrawRectTArguments args = {
        .transform = s_transform,
        .size = size,
        .color = color
    };

    QueueRenderCall(drawOrder, _DrawRectT_RenderCall, (union RenderCallFunctionArguments*)&args);
} 

static void _DrawRectW_RenderCall(union RenderCallFunctionArguments arguments)
{
    DrawRectWArguments args = arguments.drawRectWArguments;

    Raylib_DrawRectanglePro(Graphics_WorldToScreenRect(args.rect), Vector2_Zero(), 0.0f, args.color);
}

void Graphics_DrawRectW(Rect rect, Color color, i32 drawOrder)
{
    DrawRectWArguments args = {
        .rect = rect,
        .color = color
    };

    QueueRenderCall(drawOrder, _DrawRectW_RenderCall, (union RenderCallFunctionArguments*)&args);
}

void Graphics_DrawSquare(Vector2 position, float size, Color color)
{
    Graphics_DrawRect((Rect){
         .x = position.x,
         .y = position.y,
         .width = size,
         .height = size
    },
    color);
}

void Graphics_DrawSquareW(Vector2 position, float size, Color color, i32 drawOrder)
{
    Graphics_DrawRectW(Rect_FromVectors(position, Vector2_Uniform(size)), color, drawOrder);
}

void _DrawCircle_RenderCall(union RenderCallFunctionArguments arguments)
{
    DrawCircleArguments args = arguments.drawCircleArguments;
    Raylib_DrawCircleV(args.position, args.radius, args.color);
}

void Graphics_DrawCircle(Vector2 position, float radius, Color color)
{
    DrawCircleArguments args = {
        .position = position,
        .radius = radius,
        .color = color
    };

    QueueRenderCall(DRAW_ORDER_DEFAULT, _DrawCircle_RenderCall, (union RenderCallFunctionArguments*)&args);
}

void _DrawCircleT_RenderCall(union RenderCallFunctionArguments arguments)
{
    DrawCircleTArguments args = arguments.drawCircleTArguments;

    Raylib_DrawCircleV(args.transform.position, args.radius * (args.transform.scale.x + args.transform.scale.y) * 0.5f, args.color);
}

void Graphics_DrawCircleT(float radius, Color color, i32 drawOrder)
{
    DrawCircleTArguments args = {
        .transform = s_transform,
        .radius = radius,
        .color = color
    };

    QueueRenderCall(drawOrder, _DrawCircleT_RenderCall, (union RenderCallFunctionArguments*)&args);
}

void _DrawCircleW_RenderCall(union RenderCallFunctionArguments arguments)
{
    DrawCircleWArguments args = arguments.drawCircleWArguments;

    Raylib_DrawCircleV(Graphics_WorldToScreen(args.position), args.radius, args.color);
}

void Graphics_DrawCircleW(Vector2 position, float radius, Color color, i32 drawOrder)
{
    DrawCircleWArguments args = {
        .position = position,
        .radius = radius,
        .color = color
    };

    QueueRenderCall(drawOrder, _DrawCircleW_RenderCall, (union RenderCallFunctionArguments*)&args);
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

void _DrawLineSimpleW_RenderCall(union RenderCallFunctionArguments arguments)
{
    DrawLineSimpleWArguments args = arguments.drawLineSimpleWArguments;
    Raylib_DrawLineV(Graphics_WorldToScreen(args.a), Graphics_WorldToScreen(args.b), args.color);
}

void Graphics_DrawLineSimpleW(Vector2 a, Vector2 b, Color color)
{
    DrawLineSimpleWArguments args = {
        .a = a,
        .b = b,
        .color = color
    };

    QueueRenderCall(DRAW_ORDER_TOP, _DrawLineSimpleW_RenderCall, (union RenderCallFunctionArguments*)&args);
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

void _DrawLineW_RenderCall(union RenderCallFunctionArguments arguments)
{
    DrawLineWArguments args = arguments.drawLineWArguments;
    Raylib_DrawLineEx(Graphics_WorldToScreen(args.a), Graphics_WorldToScreen(args.b), args.width, args.color);
}

void Graphics_DrawLineW(Vector2 a, Vector2 b, float width, Color color)
{
    DrawLineWArguments args = {
        .a = a,
        .b = b,
        .width = width,
        .color = color
    };

    QueueRenderCall(DRAW_ORDER_TOP, _DrawLineW_RenderCall, (union RenderCallFunctionArguments*)&args);
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

void _DrawVectorW_RenderCall(union RenderCallFunctionArguments arguments)
{
    DrawVectorWArguments args = arguments.drawVectorWArguments;

    args.a = Graphics_WorldToScreen(args.a);
    args.b = Graphics_WorldToScreen(args.b);

    constexpr float arrowSize = 15.0f;
    constexpr float arrowAngle = 20.0f;
    Vector2 arrowVec = Vector2_Normalize(Vector2_Sub(args.a, args.b));

    Raylib_DrawLineV(args.a, args.b, args.color);
    Raylib_DrawLineV(args.b, Vector2_Add(args.b, Vector2_MultScalar(Vector2_Rotate(arrowVec, arrowAngle), arrowSize)), args.color);
    Raylib_DrawLineV(args.b, Vector2_Add(args.b, Vector2_MultScalar(Vector2_Rotate(arrowVec, -arrowAngle), arrowSize)), args.color);
}

void Graphics_DrawVectorW(Vector2 a, Vector2 b, Color color)
{
    DrawVectorWArguments args = {
        .a = a,
        .b = b,
        .color = color
    };

    QueueRenderCall(DRAW_ORDER_TOP, _DrawVectorW_RenderCall, (union RenderCallFunctionArguments*)&args);
}

void _DrawVectorFromPointW_RenderCall(union RenderCallFunctionArguments arguments)
{
    DrawVectorFromPointWArguments args = arguments.drawVectorFromPointWArguments;

    args.point = Graphics_WorldToScreen(args.point);
    args.vec.y = -args.vec.y;

    constexpr float arrowSize = 15.0f;
    constexpr float arrowAngle = 20.0f;
    Vector2 sum = Vector2_Add(args.point, args.vec);
    Vector2 arrowVec = Vector2_Normalize(Vector2_MultScalar(args.vec, -1.0f));

    Raylib_DrawLineV(args.point, sum, args.color);
    Raylib_DrawLineV(sum, Vector2_Add(sum, Vector2_MultScalar(Vector2_Rotate(arrowVec, arrowAngle), arrowSize)), args.color);
    Raylib_DrawLineV(sum, Vector2_Add(sum, Vector2_MultScalar(Vector2_Rotate(arrowVec, -arrowAngle), arrowSize)), args.color);
}

void Graphics_DrawVectorFromPointW(Vector2 point, Vector2 vec, Color color)
{
    DrawVectorFromPointWArguments args = {
        .point = point,
        .vec = vec,
        .color = color
    };

    QueueRenderCall(DRAW_ORDER_TOP, _DrawVectorFromPointW_RenderCall, (union RenderCallFunctionArguments*)&args);
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

void _DrawTextureW_RenderCall(union RenderCallFunctionArguments arguments)
{
    DrawTextureWArguments args = arguments.drawTextureWArguments;

    Rect src = (Rect){ .x = 0.0f, .y = 0.0f, .width = args.texture->width, .height = args.texture->height };
    Rect dest = Graphics_WorldToScreenRect(args.dest);

    Raylib_DrawTexturePro(*args.texture, src, dest, Vector2_Zero(), 0.0f, COLOR_WHITE);
}

void Graphics_DrawTextureW(const Texture2D* texture, Rect dest, i32 drawOrder)
{
    DrawTextureWArguments args = {
        .texture = texture,
        .dest = dest,
    };

    QueueRenderCall(drawOrder, _DrawTextureW_RenderCall, (union RenderCallFunctionArguments*)&args);
}

void _DrawTextureT_RenderCall(union RenderCallFunctionArguments arguments)
{
    DrawTextureTArguments args = arguments.drawTextureTArguments;

    Rect src = (Rect){ .x = 0.0f, .y = 0.0f, .width = args.texture->width, .height = args.texture->height };
    Rect dest = (Rect){ .x = args.transform.position.x, .y = args.transform.position.y, .width = args.texture->width * args.transform.scale.x, .height = args.texture->height * args.transform.scale.y };
    Vector2 origin = Vector2_Mult(WorldToScreenOrigin(args.transform.origin, (Vector2){ .x = args.texture->width, .y = args.texture->height }), args.transform.scale);

    Raylib_DrawTexturePro(*args.texture, src, dest, origin, -args.transform.rotation, COLOR_WHITE);
}

void Graphics_DrawTextureT(const Texture2D* texture, i32 drawOrder)
{
    DrawTextureTArguments args = {
        .transform = s_transform,
        .texture = texture,
    };

    QueueRenderCall(drawOrder, _DrawTextureT_RenderCall, (union RenderCallFunctionArguments*)&args);
}

void _DrawTintedTextureT_RenderCall(union RenderCallFunctionArguments arguments)
{
    DrawTintedTextureTArguments args = arguments.drawTintedTextureTArguments;

    Rect src = (Rect){ 0.0f, 0.0f, args.texture->width, args.texture->height };
    Rect dest = (Rect){ .x = args.transform.position.x, .y = args.transform.position.y, .width = args.texture->width * args.transform.scale.x, .height = args.texture->height * args.transform.scale.y };
    Vector2 origin = Vector2_Mult(WorldToScreenOrigin(args.transform.origin, (Vector2){ .x = args.texture->width, .y = args.texture->height }), args.transform.scale);

    Raylib_DrawTexturePro(*args.texture, src, dest, origin, -args.transform.rotation, args.tint);
}

void Graphics_DrawTintedTextureT(const Texture2D* texture, Color tint, i32 drawOrder)
{
    DrawTintedTextureTArguments args = {
        .transform = s_transform,
        .texture = texture,
        .tint = tint
    };

    QueueRenderCall(drawOrder, _DrawTintedTextureT_RenderCall, (union RenderCallFunctionArguments*)&args);
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

Vector2 Graphics_GetMousePositionW()
{
    return Graphics_ScreenToWorld(Raylib_GetMousePosition());
}