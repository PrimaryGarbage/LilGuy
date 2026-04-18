#ifndef __DRAW_ORDER_H__
#define __DRAW_ORDER_H__

typedef enum DrawOrder {
    DRAW_ORDER_BACKGROUND = -100,
    DRAW_ORDER_DEFAULT = 0,
    DRAW_ORDER_MAIN_CHAR_JETPACK = 99,
    DRAW_ORDER_MAIN_CHAR = 100,
    DRAW_ORDER_UI = 500,
    DRAW_ORDER_TOP = 10000,
} DrawOrder;

#endif // __DRAW_ORDER_H__