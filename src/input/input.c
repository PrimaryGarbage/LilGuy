#include "input.h"
#include "input_button.h"
#include "window/window.h"
#include "window/window_context.h"
#include "input_context.h"
#include "math_helpers.h"
#include "MiniFB.h"
#include <assert.h>

static WindowHandle Window;
static InputContext Context;

static void ClearButtonJustState()
{
    for(u32 i = 0u; i < INPUT_BUTTON_ENUM_MAX; ++i)
        Context.buttonInfo[i].just = false;
}

static void MouseMoveCallback(mfb_window* window, int x, int y)
{
    Context.mousePosition.x = Clampi(x, 0, Window->windowSize.x) + 0.5f;
    Context.mousePosition.y = Clampi(y, 0, Window->windowSize.y) + 0.5f;
}

static void MouseButtonCallback(mfb_window* window, mfb_mouse_button button, mfb_key_mod mod, bool isPressed)
{
    ButtonInfo* buttonInfo = &Context.buttonInfo[button];
    buttonInfo->just = isPressed ^ buttonInfo->pressed;
    buttonInfo->pressed = isPressed;
}

static void KeyboardButtonCallback(mfb_window* window, mfb_key key, mfb_key_mod mod, bool isPressed)
{
    ButtonInfo* buttonInfo = &Context.buttonInfo[key];
    buttonInfo->just = isPressed ^ buttonInfo->pressed;
    buttonInfo->pressed = isPressed;
}

void Input_SetWindow(WindowHandle handle)
{
    Window = handle;
    Window_AddOnUpdateCallback(handle, ClearButtonJustState);
    mfb_set_mouse_move_callback(Window->mfbWindow, MouseMoveCallback);
    mfb_set_mouse_button_callback(Window->mfbWindow, MouseButtonCallback);
    mfb_set_keyboard_callback(Window->mfbWindow, KeyboardButtonCallback);
}

bool Input_IsButtonPressed(InputButton button)
{
    assert(Window);

    return Context.buttonInfo[button].pressed;
}

bool Input_IsButtonJustPressed(InputButton button)
{
    assert(Window);

    return Context.buttonInfo[button].pressed && Context.buttonInfo[button].just;
}

Vector2 Input_GetMousePosition()
{
    assert(Window);

    return Context.mousePosition;
}
