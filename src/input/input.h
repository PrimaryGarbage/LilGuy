#ifndef __INPUT_H__
#define __INPUT_H__

#include "input_button.h"
#include "vector2.h"

bool Input_IsKeyPressed(InputKey key);
bool Input_IsKeyJustPressed(InputKey key);
bool Input_IsKeyJustReleased(InputKey key);
bool Input_IsMouseButtonPressed(InputMouseButton buton);
bool Input_IsMouseButtonJustPressed(InputMouseButton buton);
bool Input_IsMouseButtonJustReleased(InputMouseButton buton);
Vector2 Input_GetMousePosition();

#endif // __INPUT_H__