#include <SDL2/SDL.h>
#include "Arduino_Canvas.h"
#include <chrono>
#include "timer.h"

#include "../app/ui_controller.h"
#include "wifi_serial.h"
#include "platform_pc.h"

ApplicationPlatform* pPlatform;

int main(int argc, char* argv[])
{



    pPlatform = new PlatformPC();
    pPlatform->setup();
    pPlatform->loop();
    delete pPlatform;
 

    return 0;
}

