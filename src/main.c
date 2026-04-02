#include "graphics/graphics.h"
#include "random.h"
#include "timer.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main(int argc, char* argv[])
{
    RandomInit();
    Timer globalTimer = Timer_Create();
    double deltatime;

    Graphics_Init("LilGuy", WINDOW_WIDTH, WINDOW_HEIGHT);

    Image* screenCaptureImage = Graphics_GetScreenCaptureImage();

    while(Graphics_Update())
    {
        //Graphics_ClearWindow(COLOR_BLACK);
        Graphics_ClearWindowWithImage(screenCaptureImage);

        Graphics_DrawRect(Rect_CreateSquare(100.0f, 100.0f, 20.0f), COLOR_PURPLE, false);

        // draw here

        deltatime = Timer_Reset(&globalTimer);
    }

    Graphics_Deinit();

    return 0;
}