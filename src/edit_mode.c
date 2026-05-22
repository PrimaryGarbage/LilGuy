#include "edit_mode.h"
#include "cleanup.h"
#include "graphics/draw_order.h"
#include "input/input.h"
#include "input/input_button.h"
#include "graphics/graphics.h"
#include "vector2.h"

static bool s_initialized;
static bool s_enabled;
static char* s_cursorPosString;

static void Cleanup()
{
    free(s_cursorPosString);
}

static void EnsureInitialized()
{
    if (s_initialized) return;

    s_cursorPosString = malloc(128u);
    Cleanup_AddCallback(Cleanup);

    s_initialized = true;
}

void EditMode_Update(double deltatime)
{
    EnsureInitialized();

    if (Input_IsKeyJustReleased(INPUT_KEY_GRAVE)) s_enabled = !s_enabled;
}

void EditMode_Draw()
{
    if (s_enabled)
    {
        Graphics_DrawText("Edit Mode", Vector2_New(10.0f, 50.0f), 30.0f, COLOR_WHITE, DRAW_ORDER_UI);
        
        Vector2 mousePos = Input_GetMousePosition();
        sprintf(s_cursorPosString, "Cursor Pos: %d:%d", (u32)mousePos.x, (u32)mousePos.y);
        Graphics_DrawText(s_cursorPosString, Vector2_New(10.0f, 90.0f), 20.0f, COLOR_WHITE, DRAW_ORDER_UI);
    }
}

bool EditMode_Enabled()
{
    return s_enabled;
}