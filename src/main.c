#include "graphics/graphics.h"
#include "random.h"
#include "result.h"
#include "timer.h"
#include "logging.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int main(int argc, char* argv[])
{
    RandomInit();
    Timer globalTimer = Timer_Create();
    double deltatime;

    Result graphicsInitResult = Graphics_Init("LilGuy", WINDOW_WIDTH, WINDOW_HEIGHT);
    IF_ERROR_PANIC_EX(graphicsInitResult, 
        LogError(&graphicsInitResult, NULL);
    );

    Image* screenCaptureImage = Graphics_GetScreenCaptureImage();

    while(Graphics_Update())
    {
        Graphics_WaitSync();

        //Graphics_ClearWindow(COLOR_BLACK);
        Graphics_ClearWindowWithImage(screenCaptureImage);

        // draw here

        deltatime = Timer_Reset(&globalTimer);
    }

    Graphics_Deinit();

    return 0;
}