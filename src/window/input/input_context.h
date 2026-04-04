#ifndef __INPUT_CONTEXT_H__
#define __INPUT_CONTEXT_H__

#include "vector2.h"
#include "input_button.h"

typedef struct ButtonInfo {
    bool pressed;
    bool just;
} ButtonInfo;

typedef struct InputContext {
    ButtonInfo buttonInfo[INPUT_BUTTON_ENUM_MAX];
    Vector2 mousePosition;
} InputContext;

#endif // __INPUT_CONTEXT_H__