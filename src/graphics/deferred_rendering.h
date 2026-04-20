#ifndef __DELAYED_RENDERING_H__
#define __DELAYED_RENDERING_H__

#include "color.h"
#include "graphics/texture2d.h"
#include "physics/matrix.h"
#include "rect.h"
#include "short_types.h"
#include "vector2.h"

typedef enum RenderCallType {
    RENDER_CALL_TYPE_UNKNOWN = 0,
    RENDER_CALL_TYPE_DRAW_RECT,
    RENDER_CALL_TYPE_DRAW_RECT_T,
    RENDER_CALL_TYPE_DRAW_CIRCLE,
    RENDER_CALL_TYPE_DRAW_CIRCLE_T,
    RENDER_CALL_TYPE_DRAW_TEXTURE,
    RENDER_CALL_TYPE_DRAW_TEXTURE_T,
    RENDER_CALL_TYPE_DRAW_LINE_SIMPLE,
    RENDER_CALL_TYPE_DRAW_LINE,
    RENDER_CALL_TYPE_DRAW_VECTOR,
    RENDER_CALL_TYPE_DRAW_VECTOR_FROM_POINT,
} RenderCallType;

typedef struct DrawRectArguments {
    Rect rect;
    Color color;
} DrawRectArguments;

typedef struct DrawRectTArguments {
    Vector2 size;
    Color color;
    Matrix matrix;
} DrawRectTArguments;

typedef struct DrawCircleArguments {
    Vector2 position;
    float radius;
    Color color;
} DrawCircleArguments;

typedef struct DrawCircleTArguments {
    float radius;
    Color color;
    Matrix matrix;
} DrawCircleTArguments;

typedef struct DrawTextureArguments {
    const Texture2D* texture;
    Rect dest;
} DrawTextureArguments;

typedef struct DrawTextureTArguments {
    const Texture2D* texture;
    Color tint;
    Matrix matrix;
} DrawTextureTArguments;

typedef struct DrawLineSimpleArguments {
    Vector2 a;
    Vector2 b;
    Color color;
} DrawLineSimpleArguments;

typedef struct DrawLineArguments {
    Vector2 a;
    Vector2 b;
    float width;
    Color color;
} DrawLineArguments;

typedef struct DrawVectorArguments {
    Vector2 a;
    Vector2 b;
    Color color;
} DrawVectorArguments;

typedef struct DrawVectorFromPointArguments {
    Vector2 point;
    Vector2 vec;
    Color color;
} DrawVectorFromPointArguments;

union RenderCallFunctionArguments {
    DrawRectArguments drawRectArguments;
    DrawRectTArguments drawRectTArguments;
    DrawCircleArguments drawCircleArguments;
    DrawCircleTArguments drawCircleTArguments;
    DrawTextureArguments drawTextureArguments;
    DrawTextureTArguments drawTextureTArguments;
    DrawLineSimpleArguments drawLineSimpleArguments;
    DrawLineArguments drawLineArguments;
    DrawVectorArguments drawVectorArguments;
    DrawVectorFromPointArguments drawVectorFromPointArguments;
};

typedef void(*RenderCallFunction)(union RenderCallFunctionArguments arguments);

typedef struct RenderCall {
    i32 drawOrder;
    u32 drawIndex;
    RenderCallFunction function;
    union RenderCallFunctionArguments functionArguments;
} RenderCall;

static inline int RenderCallComparer(const void* a, const void* b)
{
    if (((RenderCall*)a)->drawOrder == ((RenderCall*)b)->drawOrder)
        return ((RenderCall*)a)->drawIndex - ((RenderCall*)b)->drawIndex;
    else
        return ((RenderCall*)a)->drawOrder - ((RenderCall*)b)->drawOrder;
}

#endif // __DELAYED_RENDERING_H__