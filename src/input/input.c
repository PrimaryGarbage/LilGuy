#include "input.h"
#include "input_button.h"
#include "raylib_wrapper.h"
#include <assert.h>

bool Input_IsKeyPressed(InputKey key)
{
    return Raylib_IsKeyDown(key);
}

bool Input_IsKeyJustPressed(InputKey key)
{
    return Raylib_IsKeyPressed(key);
}

bool Input_IsKeyJustReleased(InputKey key)
{
    return Raylib_IsKeyReleased(key);
}

bool Input_IsMouseButtonPressed(InputMouseButton button)
{
    return Raylib_IsMouseButtonDown(button);
}

bool Input_IsMouseButtonJustPressed(InputMouseButton button)
{
    return Raylib_IsMouseButtonPressed(button);
}

bool Input_IsMouseButtonJustReleased(InputMouseButton button)
{
    return Raylib_IsMouseButtonReleased(button);
}

Vector2 Input_GetMousePosition()
{
    return Raylib_GetMousePosition();
}
