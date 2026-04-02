#include "logging.h"
#include "vector2.h"
#include "graphics/graphics.h"
#include "random.h"
#include <stdlib.h>
#include <time.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

static time_t StartTime;

static time_t LastTrigger;
void drawLilPresentation()
{
    static time_t triggerInterval = 5;
    static u8 trigger = 0;
    static float triggerCurtainYPos = 0.0f;

    if (trigger < 4)
    {
        if (time(NULL) - LastTrigger > triggerInterval) 
        {
            trigger++;
            LastTrigger = time(NULL);
        }
    }
    else
    {
        triggerCurtainYPos += 5.0f;
        if (triggerCurtainYPos > WINDOW_HEIGHT)
        {
            triggerCurtainYPos = 0.0f;
            trigger = 0;
        }
    }

    Vector2 mousePos = graphics_getMousePosition();

    u32 size = randomRange(1, 50);
    Vector2 pos = { randomRange(0, WINDOW_WIDTH + size), randomRange(0, WINDOW_HEIGHT + size) };

    switch(trigger)
    {
        case 0:
            graphics_drawSquare(pos, size, (Color){ randomFloat(), randomFloat(), randomFloat(), randomFloat() }, true);
            break;
        case 1:
            graphics_drawCircle(pos, size, (Color){ randomFloat(), randomFloat(), randomFloat(), randomFloat() }, true);
            break;
        case 2:
            graphics_drawSquare(pos, size, (Color){ randomFloat(), randomFloat(), randomFloat(), randomFloat() }, false);
            break;
        case 3:
            graphics_drawCircle(pos, size, (Color){ randomFloat(), randomFloat(), randomFloat(), randomFloat() }, false);
            break;
        case 4:
            graphics_drawCircle(pos, size, (Color){ randomFloat(), randomFloat(), randomFloat(), randomFloat() }, false);
            graphics_drawRect((Rect){ (Vector2){ 0.0f, 0.0f }, (Vector2){ WINDOW_WIDTH + 100, triggerCurtainYPos } }, COLOR_BLACK, false);
            break;
    }
}

int main(int argc, char* argv[])
{
    randomInit();
    StartTime = time(NULL);
    LastTrigger = StartTime;

    graphics_init("LilGuy", WINDOW_WIDTH, WINDOW_HEIGHT);

    graphics_clearWindow(COLOR_BLACK);

    while(graphics_update())
    {
        //graphics_clearWindow(COLOR_BLACK);

        drawLilPresentation();
    }

    graphics_deinit();

    return 0;
}