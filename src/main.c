#include "graphics/graphics.h"
#include "random.h"
#include "timer.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main(int argc, char* argv[])
{
    RandomInit();
    Timer globalTimer = Timer_Create();

    Graphics_Init("LilGuy", WINDOW_WIDTH, WINDOW_HEIGHT);

    while(Graphics_Update())
    {
        Graphics_ClearWindow(COLOR_BLACK);

        // draw here

        double deltatime = Timer_Reset(&globalTimer);
    }

    Graphics_Deinit();

    return 0;
}