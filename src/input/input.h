#ifndef __INPUT_H__
#define __INPUT_H__

#include "input_button.h"
#include "vector2.h"

typedef struct WindowContext* WindowHandle;

void Input_SetWindow(WindowHandle handle);
bool Input_IsButtonPressed(InputButton button);
bool Input_IsButtonJustPressed(InputButton button);
Vector2 Input_GetMousePosition();

#endif // __INPUT_H__